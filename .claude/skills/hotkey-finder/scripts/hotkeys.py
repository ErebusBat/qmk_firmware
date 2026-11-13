#!/usr/bin/env -S uv run --script
# /// script
# requires-python = ">=3.11"
# dependencies = ["cryptography"]
# ///
"""Inventory the global hotkeys in use on this Mac and find free chords.

Sources (each is optional; failures are reported, not fatal):
  raycast  newest .rayconfig export (RAYCFG3 and legacy openssl formats), decrypted in memory
  k2he     K2 HE QMK keymap read from a git ref (default k2_he_2025q3)
  planck   Planck EZ layout fetched live from the ZSA Oryx GraphQL API
  mytts    MyTTS shortcut bindings (defaults domain net.erebusbat.mytts)
  macos    enabled entries in com.apple.symbolichotkeys, plus a short built-in reserved list

Receivers (raycast, mytts, macos) act on a chord; senders (k2he, planck) emit it.
"""

from __future__ import annotations

import argparse
import gzip
import hashlib
import json
import os
import plistlib
import re
import struct
import subprocess
import sys
import urllib.request
from collections import defaultdict
from dataclasses import dataclass, field
from pathlib import Path

RAYCAST_EXPORT_DIR = Path.home() / "Documents/Library/R/Raycast Exports"
RAYCAST_PASSPHRASE_OP_REF = "op://Private/47xuupmqdzuswcp6umnlkq43y4/password"  # "Raycast Backup/ Export Password"
K2HE_REF = "k2_he_2025q3"
K2HE_KEYMAP_DIR = "keyboards/keychron/k2_he/ansi/keymaps/erebusbat"
ORYX_URL = "https://oryx.zsa.io/graphql"
PLANCK_LAYOUT = ("BqKMO", "planck-ez", "latest")

MOD_ORDER = ("ctrl", "opt", "shift", "cmd")
HYPER = frozenset(MOD_ORDER)
MEH = frozenset(("ctrl", "opt", "shift"))

# macOS virtual keycodes (kVK_*); Carbon keycodes are the same values.
KVK = {
    0: "A", 1: "S", 2: "D", 3: "F", 4: "H", 5: "G", 6: "Z", 7: "X", 8: "C", 9: "V", 11: "B", 12: "Q",
    13: "W", 14: "E", 15: "R", 16: "Y", 17: "T", 18: "1", 19: "2", 20: "3", 21: "4", 22: "6", 23: "5",
    24: "=", 25: "9", 26: "7", 27: "-", 28: "8", 29: "0", 30: "]", 31: "O", 32: "U", 33: "[", 34: "I",
    35: "P", 36: "Return", 37: "L", 38: "J", 39: "'", 40: "K", 41: ";", 42: "\\", 43: ",", 44: "/",
    45: "N", 46: "M", 47: ".", 48: "Tab", 49: "Space", 50: "`", 51: "Backspace", 53: "Escape",
    64: "F17", 65: "Keypad.", 67: "Keypad*", 69: "Keypad+", 71: "KeypadClear", 75: "Keypad/",
    76: "KeypadEnter", 78: "Keypad-", 79: "F18", 80: "F19", 81: "Keypad=", 82: "Keypad0", 83: "Keypad1",
    84: "Keypad2", 85: "Keypad3", 86: "Keypad4", 87: "Keypad5", 88: "Keypad6", 89: "Keypad7", 90: "F20",
    91: "Keypad8", 92: "Keypad9", 96: "F5", 97: "F6", 98: "F7", 99: "F3", 100: "F8", 101: "F9",
    103: "F11", 105: "F13", 106: "F16", 107: "F14", 109: "F10", 111: "F12", 113: "F15", 114: "Help",
    115: "Home", 116: "PageUp", 117: "Delete", 118: "F4", 119: "End", 120: "F2", 121: "PageDown",
    122: "F1", 123: "Left", 124: "Right", 125: "Down", 126: "Up",
}

QMK_KEYS = {
    **{f"KC_{c}": c for c in "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"},
    **{f"KC_F{n}": f"F{n}" for n in range(1, 25)},
    "KC_ENT": "Return", "KC_ENTER": "Return", "KC_ESC": "Escape", "KC_ESCAPE": "Escape",
    "KC_BSPC": "Backspace", "KC_BACKSPACE": "Backspace", "KC_TAB": "Tab", "KC_SPC": "Space",
    "KC_SPACE": "Space", "KC_MINS": "-", "KC_MINUS": "-", "KC_EQL": "=", "KC_EQUAL": "=",
    "KC_LBRC": "[", "KC_LEFT_BRACKET": "[", "KC_RBRC": "]", "KC_RIGHT_BRACKET": "]",
    "KC_BSLS": "\\", "KC_BACKSLASH": "\\", "KC_SCLN": ";", "KC_SEMICOLON": ";", "KC_QUOT": "'",
    "KC_QUOTE": "'", "KC_GRV": "`", "KC_GRAVE": "`", "KC_COMM": ",", "KC_COMMA": ",", "KC_DOT": ".",
    "KC_SLSH": "/", "KC_SLASH": "/", "KC_DEL": "Delete", "KC_DELETE": "Delete", "KC_HOME": "Home",
    "KC_END": "End", "KC_PGUP": "PageUp", "KC_PAGE_UP": "PageUp", "KC_PGDN": "PageDown",
    "KC_PAGE_DOWN": "PageDown", "KC_LEFT": "Left", "KC_RGHT": "Right", "KC_RIGHT": "Right",
    "KC_UP": "Up", "KC_DOWN": "Down",
}

# QMK modifier-wrapper functions -> modifiers added (left/right folded together).
QMK_WRAPPERS = {
    **dict.fromkeys(("C", "LCTL", "RCTL"), {"ctrl"}),
    **dict.fromkeys(("S", "LSFT", "RSFT"), {"shift"}),
    **dict.fromkeys(("A", "LALT", "RALT", "LOPT", "ROPT", "ALGR"), {"opt"}),
    **dict.fromkeys(("G", "LGUI", "RGUI", "LCMD", "RCMD", "LWIN", "RWIN"), {"cmd"}),
    **dict.fromkeys(("LCA", "RCA"), {"ctrl", "opt"}),
    **dict.fromkeys(("LCS", "RCS"), {"ctrl", "shift"}),
    **dict.fromkeys(("LCG", "RCG"), {"ctrl", "cmd"}),
    **dict.fromkeys(("LSA", "RSA"), {"shift", "opt"}),
    **dict.fromkeys(("LSG", "RSG", "SGUI", "SCMD", "SWIN"), {"shift", "cmd"}),
    **dict.fromkeys(("LAG", "RAG"), {"opt", "cmd"}),
    **dict.fromkeys(("LCAG", "RCAG"), {"ctrl", "opt", "cmd"}),
    **dict.fromkeys(("LSAG", "RSAG"), {"shift", "opt", "cmd"}),
    "MEH": set(MEH),
    "HYPR": set(HYPER),
}

# Well-known com.apple.symbolichotkeys ids; unknown ids print as a bare number.
SYMBOLIC_NAMES = {
    28: "(screenshot to file)", 29: "(screenshot to clipboard)", 30: "(area screenshot to file)",
    31: "(area screenshot to clipboard)", 32: "(Mission Control)", 33: "(application windows)",
    36: "(show desktop)", 60: "(previous input source)", 61: "(next input source)", 64: "(Spotlight)",
    65: "(Finder search window)", 79: "(move left a space)", 80: "(move left a space)",
    81: "(move right a space)", 82: "(move right a space)", 190: "(Quick Note)",
}

# Built-in macOS shortcuts worth avoiding even when absent from symbolichotkeys.
RESERVED = {
    (HYPER, "."): "macOS sysdiagnose",
    (frozenset({"cmd"}), "Space"): "Spotlight / launcher",
    (frozenset({"cmd", "shift"}), "3"): "macOS screenshot",
    (frozenset({"cmd", "shift"}), "4"): "macOS screenshot",
    (frozenset({"cmd", "shift"}), "5"): "macOS screenshot",
    (frozenset({"ctrl", "cmd"}), "Q"): "macOS lock screen",
    (frozenset({"ctrl", "cmd"}), "Space"): "macOS emoji picker",
    (frozenset({"opt", "cmd"}), "Escape"): "macOS force quit",
    (frozenset(), "F14"): "macOS brightness down",
    (frozenset(), "F15"): "macOS brightness up",
}


@dataclass(frozen=True)
class Chord:
    mods: frozenset
    key: str

    def __str__(self) -> str:
        if self.mods == HYPER:
            prefix = "Hyper"
        elif self.mods == MEH:
            prefix = "Meh"
        else:
            prefix = "+".join(m.capitalize() for m in MOD_ORDER if m in self.mods)
        return f"{prefix}+{self.key}" if prefix else self.key


@dataclass
class Inventory:
    receivers: dict = field(default_factory=lambda: defaultdict(list))  # Chord -> [str]
    senders: dict = field(default_factory=lambda: defaultdict(list))  # Chord -> [str]
    other: list = field(default_factory=list)  # non-chord hotkeys (double-taps, etc.)
    status: list = field(default_factory=list)  # source status lines


# ---------------------------------------------------------------------------- Raycast


def raycast_passphrase() -> str:
    if pw := os.environ.get("RAYCAST_BACKUP_PASSWORD"):
        return pw
    out = subprocess.run(["op", "read", RAYCAST_PASSPHRASE_OP_REF], capture_output=True, text=True)
    if out.returncode != 0:
        raise RuntimeError(f"op read failed (unlock 1Password or set RAYCAST_BACKUP_PASSWORD): {out.stderr.strip()}")
    return out.stdout.rstrip("\n")


def decrypt_rayconfig(path: Path, passphrase: str) -> tuple[dict, str]:
    from cryptography.hazmat.primitives import padding
    from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
    from cryptography.hazmat.primitives.ciphers.aead import AESGCM

    data = path.read_bytes()
    pw = passphrase.encode()
    if data.startswith(b"RAYCFG3\n"):
        # Raycast 2.x: magic, u32le header length, gzip(JSON header with iv+salt), AES-256-GCM body.
        (n,) = struct.unpack("<I", data[8:12])
        meta = json.loads(gzip.decompress(data[12 : 12 + n]))
        enc = meta["encryption"]
        key = hashlib.scrypt(pw, salt=bytes.fromhex(enc["salt"]), n=16384, r=8, p=1, dklen=32)
        body = AESGCM(key).decrypt(bytes.fromhex(enc["iv"]), data[12 + n :], None)
        return json.loads(gzip.decompress(body)), meta.get("exportedAt", "?")
    # Legacy: `openssl enc -aes-256-cbc -nosalt -md sha256`, 16-byte header, then gzip.
    d, prev = b"", b""
    while len(d) < 48:
        prev = hashlib.sha256(prev + pw).digest()
        d += prev
    dec = Cipher(algorithms.AES(d[:32]), modes.CBC(d[32:48])).decryptor()
    plain = dec.update(data) + dec.finalize()
    unpadder = padding.PKCS7(128).unpadder()
    plain = unpadder.update(plain) + unpadder.finalize()
    from datetime import datetime

    return json.loads(gzip.decompress(plain[16:])), datetime.fromtimestamp(path.stat().st_mtime).isoformat(timespec="seconds")


def raycast_v2_hotkey(hk: dict):
    kind = hk.get("kind", {})
    sc = kind.get("shortcut", {})
    if kind.get("type") == "SingleStep" and "key" in sc:
        mods = frozenset({"Ctrl": "ctrl", "Alt": "opt", "Shift": "shift", "Meta": "cmd"}[m["modifier"]] for m in sc.get("modifiers", []))
        k = sc["key"]
        key = KVK.get(k["code"], f"kVK{k['code']}") if "code" in k else str(k.get("character", k)).upper()
        return Chord(mods, key)
    if kind.get("type") == "DoubleTap" and "modifier" in sc:
        m = sc["modifier"]
        return f"double-tap {m.get('directionality', '')} {m['modifier']}".replace("  ", " ")
    return json.dumps(hk)


def raycast_command_name(cid: str, ext_names: dict) -> str:
    if "::application::=::" in cid:
        return "Open " + Path(cid.rsplit("::", 1)[-1]).stem
    parts = cid.split("::")
    ext = parts[0].split(":", 2)[-1]
    ext = ext_names.get(ext, ext)
    return f"{ext}: {parts[-1]}" if len(parts) > 1 else ext


def load_raycast(inv: Inventory, export: Path | None) -> None:
    if export is None:
        exports = sorted(RAYCAST_EXPORT_DIR.glob("*.rayconfig"), key=lambda p: p.stat().st_mtime)
        if not exports:
            raise RuntimeError(f"no .rayconfig in {RAYCAST_EXPORT_DIR}")
        export = exports[-1]
    j, exported_at = decrypt_rayconfig(export, raycast_passphrase())
    count = 0

    def add(hk, name):
        nonlocal count
        count += 1
        (inv.receivers[hk].append if isinstance(hk, Chord) else lambda n: inv.other.append(f"{hk}  ->  {n}"))(f"Raycast: {name}")

    if "settings" in j:  # Raycast 2.x schema
        ext_names = {e["uuid"]: e.get("name", e["uuid"]) for e in j.get("nodeExtensions", {}).get("extensions", [])}
        if g := j["settings"].get("general", {}).get("globalHotkey"):
            add(raycast_v2_hotkey(g), "launcher (global hotkey)")
        for c in j["settings"].get("commands", []):
            if c.get("macosHotkey") and c.get("enabled", True):
                add(raycast_v2_hotkey(c["macosHotkey"]), raycast_command_name(c["id"], ext_names))
    else:  # legacy schema
        mods_map = {"Shift": "shift", "Control": "ctrl", "Option": "opt", "Command": "cmd"}

        def parse(s):
            *mods, code = s.split("-")
            return Chord(frozenset(mods_map[m] for m in mods), KVK.get(int(code), code)) if code.isdigit() else s

        if g := j.get("builtin_package_raycastPreferences", {}).get("preferencesGeneral", {}).get("raycastGlobalHotkey"):
            add(parse(g), "launcher (global hotkey)")
        for r in j.get("builtin_package_rootSearch", {}).get("rootSearch", []):
            if r.get("hotkey"):
                add(parse(r["hotkey"]), Path(r["path"]).stem if r.get("path") else r.get("key", "?"))
    inv.status.append(f"raycast: {count} hotkeys from '{export.name}' (exported {exported_at}; a snapshot, not live settings)")


# ---------------------------------------------------------------------------- QMK (K2 HE)


def strip_c_comments(src: str) -> str:
    src = re.sub(r"/\*.*?\*/", " ", src, flags=re.S)
    return re.sub(r"//[^\n]*", "", src)


def split_top_level(s: str) -> list[str]:
    out, depth, cur = [], 0, []
    for ch in s:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            out.append("".join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    if "".join(cur).strip():
        out.append("".join(cur).strip())
    return out


def qmk_eval(expr: str, defines: dict, depth: int = 0) -> Chord | None:
    expr = expr.strip()
    if depth > 20:
        return None
    if expr in QMK_KEYS:
        return Chord(frozenset(), QMK_KEYS[expr])
    if expr in defines:
        return qmk_eval(defines[expr], defines, depth + 1)
    m = re.fullmatch(r"(\w+)\((.*)\)", expr, flags=re.S)
    if m and m.group(1) in QMK_WRAPPERS:
        inner = qmk_eval(m.group(2), defines, depth + 1)
        if inner:
            return Chord(inner.mods | frozenset(QMK_WRAPPERS[m.group(1)]), inner.key)
    return None


def interesting(c: Chord | None) -> bool:
    """Chords worth tracking: modified keys, or bare F13+ (which only exist to be bound)."""
    return bool(c) and (bool(c.mods) or bool(re.fullmatch(r"F(1[3-9]|2\d)", c.key)))


def load_k2he(inv: Inventory, ref: str) -> None:
    def show(name):
        r = subprocess.run(["git", "show", f"{ref}:{K2HE_KEYMAP_DIR}/{name}"], capture_output=True, text=True)
        if r.returncode != 0:
            raise RuntimeError(r.stderr.strip())
        return r.stdout

    sha = subprocess.run(["git", "rev-parse", "--short", ref], capture_output=True, text=True).stdout.strip()
    header, keymap = show("erebusbat.h"), show("keymap.c")
    defines = {}
    for src in (header, keymap):
        for m in re.finditer(r"^[ \t]*#define[ \t]+(\w+)[ \t]+((?:[^\n]*\\\n)*[^\n]*)", src, flags=re.M):
            defines[m.group(1)] = strip_c_comments(m.group(2).replace("\\\n", " ")).strip()

    code = strip_c_comments(keymap)
    placed = defaultdict(set)  # define/expression -> layers
    for m in re.finditer(r"\[(\w+)\]\s*=\s*LAYOUT\w*\(", code):
        i, depth = m.end(), 1
        while depth:
            depth += {"(": 1, ")": -1}.get(code[i], 0)
            i += 1
        for arg in split_top_level(code[m.end() : i - 1]):
            placed[arg].add(m.group(1))
    keymaps_span = re.search(r"keymaps\s*\[\]\[MATRIX_ROWS\]\[MATRIX_COLS\]\s*=\s*\{.*?\n\};", code, flags=re.S)
    outside = code.replace(keymaps_span.group(0), "") if keymaps_span else code

    count = 0
    for expr, layers in placed.items():
        c = qmk_eval(expr, defines)
        if interesting(c):
            inv.senders[c].append(f"K2HE {expr} (layer {', '.join(sorted(layers))})")
            count += 1
    for name, body in defines.items():
        if name in placed:
            continue
        c = qmk_eval(name, defines)
        if interesting(c):
            where = "used in keymap.c code" if re.search(rf"\b{name}\b", outside) else "defined, not placed on a key"
            inv.senders[c].append(f"K2HE {name} ({where})")
            count += 1
    inv.status.append(f"k2he: {count} chord keycodes from {ref}@{sha}")


# ---------------------------------------------------------------------------- Planck EZ (Oryx)


def load_planck(inv: Inventory) -> None:
    hash_id, geometry, revision = PLANCK_LAYOUT
    query = (
        "query($h:String!,$r:String!,$g:String){layout(hashId:$h,geometry:$g,revisionId:$r)"
        "{title revision{hashId createdAt layers{title position keys}}}}"
    )
    req = urllib.request.Request(
        ORYX_URL,
        data=json.dumps({"query": query, "variables": {"h": hash_id, "g": geometry, "r": revision}}).encode(),
        headers={"content-type": "application/json"},
    )
    with urllib.request.urlopen(req, timeout=20) as resp:
        j = json.load(resp)
    if j.get("errors"):
        raise RuntimeError(j["errors"])
    lay = j["data"]["layout"]
    rev = lay["revision"]
    mod_map = {"Ctrl": "ctrl", "Alt": "opt", "Shift": "shift", "Gui": "cmd"}
    count = 0
    for layer in rev["layers"]:
        for pos, k in enumerate(layer["keys"]):
            for slot in ("tap", "hold", "doubleTap", "tapHold"):
                a = k.get(slot)
                if not a or a.get("code") not in QMK_KEYS:
                    continue
                mods = frozenset(v for flag, on in (a.get("modifiers") or {}).items() if on for s, v in mod_map.items() if flag.endswith(s))
                c = Chord(mods, QMK_KEYS[a["code"]])
                if interesting(c):
                    label = k.get("customLabel") or f"key {pos}"
                    inv.senders[c].append(f"Planck {layer['title']} '{label}' {slot}")
                    count += 1
    inv.status.append(f"planck: {count} chord keys from Oryx '{lay['title']}' revision {rev['hashId']} ({rev['createdAt']})")


# ---------------------------------------------------------------------------- MyTTS / macOS


def defaults_export(domain: str) -> dict:
    out = subprocess.run(["defaults", "export", domain, "-"], capture_output=True)
    return plistlib.loads(out.stdout) if out.returncode == 0 and out.stdout else {}


def load_mytts(inv: Inventory) -> None:
    raw = defaults_export("net.erebusbat.mytts").get("reader.shortcuts")
    if not raw:
        inv.status.append("mytts: no reader.shortcuts found")
        return
    carbon = {256: "cmd", 512: "shift", 2048: "opt", 4096: "ctrl"}
    bindings = json.loads(raw)["bindings"]
    for action, b in bindings.items():
        mods = frozenset(v for bit, v in carbon.items() if b["carbonModifiers"] & bit)
        inv.receivers[Chord(mods, KVK.get(b["carbonKeyCode"], str(b["carbonKeyCode"])))].append(f"MyTTS: {action}")
    inv.status.append(f"mytts: {len(bindings)} bindings (live defaults)")


def load_macos(inv: Inventory) -> None:
    flags = {131072: "shift", 262144: "ctrl", 524288: "opt", 1048576: "cmd"}
    hk = defaults_export("com.apple.symbolichotkeys").get("AppleSymbolicHotKeys", {})
    count = 0
    for hid, v in hk.items():
        params = v.get("value", {}).get("parameters", [])
        if v.get("enabled") and len(params) == 3 and params[1] in KVK:
            mods = frozenset(name for bit, name in flags.items() if params[2] & bit)
            name = SYMBOLIC_NAMES.get(int(hid), "")
            inv.receivers[Chord(mods, KVK[params[1]])].append(f"macOS symbolic hotkey #{hid} {name}".rstrip())
            count += 1
    for (mods, key), what in RESERVED.items():
        inv.receivers[Chord(mods, key)].append(f"reserved: {what}")
    inv.status.append(f"macos: {count} enabled symbolic hotkeys (live) + {len(RESERVED)} built-in reserved chords")


# ---------------------------------------------------------------------------- commands

MOD_ALIASES = {
    "ctrl": "ctrl", "control": "ctrl", "c": "ctrl", "⌃": "ctrl",
    "opt": "opt", "option": "opt", "alt": "opt", "a": "opt", "⌥": "opt",
    "shift": "shift", "s": "shift", "⇧": "shift",
    "cmd": "cmd", "command": "cmd", "gui": "cmd", "meta": "cmd", "g": "cmd", "⌘": "cmd",
}
KEY_ALIASES = {"enter": "Return", "ret": "Return", "esc": "Escape", "bspc": "Backspace", "del": "Delete",
               "pgup": "PageUp", "pgdn": "PageDown", "minus": "-", "equal": "=", "comma": ",", "period": ".",
               "dot": ".", "slash": "/", "backslash": "\\", "semicolon": ";", "quote": "'", "grave": "`"}


def parse_chord(text: str) -> Chord:
    *mod_parts, key = [p for p in re.split(r"\+", text.strip()) if p] or [text]
    mods = set()
    for p in mod_parts:
        p = p.strip().lower()
        if p == "hyper":
            mods |= HYPER
        elif p == "meh":
            mods |= MEH
        elif p.upper() == "C⌥⌘":
            mods |= {"ctrl", "opt", "cmd"}
        elif all(ch in MOD_ALIASES for ch in p) and p not in MOD_ALIASES:
            mods |= {MOD_ALIASES[ch] for ch in p}  # e.g. "⌃⌥⌘"
        elif p in MOD_ALIASES:
            mods.add(MOD_ALIASES[p])
        else:
            raise SystemExit(f"unknown modifier '{p}' in '{text}'")
    k = key.strip()
    k = KEY_ALIASES.get(k.lower(), k)
    names = {v.upper(): v for v in KVK.values()} | {f"F{n}": f"F{n}" for n in range(1, 25)}
    return Chord(frozenset(mods), names.get(k.upper(), k.upper() if len(k) == 1 else k))


def build(args) -> Inventory:
    inv = Inventory()
    loaders = {
        "raycast": lambda: load_raycast(inv, args.export),
        "k2he": lambda: load_k2he(inv, args.k2he_ref),
        "planck": lambda: load_planck(inv),
        "mytts": lambda: load_mytts(inv),
        "macos": lambda: load_macos(inv),
    }
    for name, fn in loaders.items():
        if name in args.skip:
            inv.status.append(f"{name}: skipped")
            continue
        try:
            fn()
        except Exception as e:  # report and keep going; a partial inventory is still useful
            inv.status.append(f"{name}: FAILED - {e}")
    return inv


def chord_sort_key(c: Chord):
    return (-len(c.mods), [m not in c.mods for m in MOD_ORDER], c.key)


def cmd_inventory(inv: Inventory, args) -> None:
    chords = sorted(set(inv.receivers) | set(inv.senders), key=chord_sort_key)
    for c in chords:
        recv, send = inv.receivers.get(c, []), inv.senders.get(c, [])
        if args.conflicts and len(recv) < 2:
            continue
        flag = "CONFLICT " if len(recv) > 1 else "UNBOUND " if send and not recv and global_looking(c) else ""
        print(f"{flag}{c}")
        for r in recv:
            print(f"    <- {r}")
        for s in send:
            print(f"    -> sent by {s}")
    if inv.other and not args.conflicts:
        print("\nNon-chord hotkeys:")
        for o in inv.other:
            print(f"    {o}")


def global_looking(c: Chord) -> bool:
    """Chords only a global hotkey would plausibly consume (not app-level shortcuts like Cmd+C)."""
    return len(c.mods) >= 3 or (not c.mods and interesting(c))


def cmd_check(inv: Inventory, args) -> None:
    for text in args.chords:
        c = parse_chord(text)
        recv, send = inv.receivers.get(c, []), inv.senders.get(c, [])
        print(f"{c}: {'TAKEN' if recv or send else 'FREE'}")
        for r in recv:
            print(f"    <- {r}")
        for s in send:
            print(f"    -> sent by {s}")


def cmd_free(inv: Inventory, args) -> None:
    taken = set(inv.receivers) | set(inv.senders)
    letters = list("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
    digits = list("1234567890")
    punct = ["-", "=", "[", "]", "\\", ";", "'", ",", "/", "`"]
    f_hi = [f"F{n}" for n in range(13, 21)]
    f_lo = [f"F{n}" for n in range(1, 13)]
    nav = ["Home", "End", "PageUp", "PageDown", "Up", "Down", "Left", "Right", "Return", "Space", "Tab", "Delete"]
    meh_like = [MEH, frozenset({"ctrl", "shift", "cmd"}), frozenset({"opt", "shift", "cmd"})]
    lca = frozenset({"ctrl", "opt", "cmd"})
    if args.style == "macro":
        pools = [
            ("bare F13-F20", [frozenset()], f_hi),
            ("Hyper / Ctrl+Opt+Cmd + F13-F20", [HYPER, lca], f_hi),
            ("Meh-like + F13-F20", meh_like, f_hi),
            ("Hyper / Ctrl+Opt+Cmd + F1-F12", [HYPER, lca], f_lo),
            ("Hyper / Ctrl+Opt+Cmd + punctuation & nav", [HYPER, lca], punct + nav),
        ]
    else:
        pools = [
            ("Hyper + letter", [HYPER], letters),
            ("Hyper + digit", [HYPER], digits),
            ("Hyper + punctuation", [HYPER], punct),
            ("Ctrl+Opt+Cmd + letter", [lca], letters),
        ]
    for title, modsets, keys in pools:
        free = [Chord(m, k) for m in modsets for k in keys if Chord(m, k) not in taken]
        print(f"{title}: {len(free)} free")
        if free:
            print("    " + "  ".join(str(c) for c in free[: args.limit]) + ("  ..." if len(free) > args.limit else ""))


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--export", type=Path, help="specific .rayconfig (default: newest by mtime)")
    ap.add_argument("--k2he-ref", default=K2HE_REF, help=f"git ref holding the K2 HE keymap (default {K2HE_REF})")
    ap.add_argument("--skip", action="append", default=[], choices=["raycast", "k2he", "planck", "mytts", "macos"],
                    help="skip a source (repeatable)")
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("inventory", help="list every known chord with its receivers and senders")
    p.add_argument("--conflicts", action="store_true", help="only chords with more than one receiver")
    p = sub.add_parser("check", help="report owners of specific chords, e.g. 'hyper+3' 'ctrl+opt+cmd+f10'")
    p.add_argument("chords", nargs="+")
    p = sub.add_parser("free", help="list free chords from a candidate pool")
    p.add_argument("--style", choices=["macro", "human"], default="macro")
    p.add_argument("--limit", type=int, default=40, help="max chords shown per pool")
    args = ap.parse_args()

    inv = build(args)
    print("Sources:")
    for s in inv.status:
        print(f"  {s}")
    print()
    {"inventory": cmd_inventory, "check": cmd_check, "free": cmd_free}[args.cmd](inv, args)


if __name__ == "__main__":
    main()
