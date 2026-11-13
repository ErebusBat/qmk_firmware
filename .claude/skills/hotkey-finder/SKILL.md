---
name: hotkey-finder
description: Find a free global hotkey to program into the K2 HE or Planck EZ, check whether a chord is already taken, or audit hotkey conflicts across Raycast, MyTTS, macOS, and both keyboard keymaps.
---

# Hotkey finder

Two kinds of chord:

- **Macro** (default): the keyboard firmware emits it from one keypress. Awkwardness is free, so spend obscure chords here.
- **Human**: the user presses it by hand. Use this only when the user says they will type it.

Every chord has **receivers** (Raycast, MyTTS, macOS act on it) and **senders** (K2 HE, Planck EZ keys emit it). A chord is free only when it has neither.

## Steps

1. **Inventory.** Run from the repo root:

   ```bash
   uv run --script .claude/skills/hotkey-finder/scripts/hotkeys.py free --style macro   # or --style human
   uv run --script .claude/skills/hotkey-finder/scripts/hotkeys.py check 'hyper+f13' 'ctrl+opt+cmd+;'
   uv run --script .claude/skills/hotkey-finder/scripts/hotkeys.py inventory [--conflicts]
   ```

   Read the `Sources:` block first. If a source shows `FAILED`, the answer is partial. Fix the cause (`op` needs 1Password unlocked; Planck needs network to reach Oryx) or name the missing source in your answer.
2. **Pick** a chord using the rules for its kind (below). For a related set (e.g. new window positions), pick adjacent keys under one modifier family.
3. **Verify.** `check` your pick and 2 alternates. Each must print `FREE`. In `inventory`, `UNBOUND` chords still count as taken: a keyboard sends them to an app this script cannot see (e.g. Hyper+\ goes to dictation).
4. **Report** the recommended chord, the alternates, the kind, and the snapshot dates from `Sources:`. The Raycast export is a snapshot: assignments made after it are invisible, so tell the user how old it is. Mention any `CONFLICT` you saw on chords near the pick.
5. **Program it** only when asked:
   - **K2 HE:** in the `k2_he_2025q3` worktree (`../k2_he`), add a `#define` to `keyboards/keychron/k2_he/ansi/keymaps/erebusbat/erebusbat.h` using QMK wrappers (`HYPR()`, `LCAG()`, `MEH()`), then place it in `keymap.c`. Build and flash per `AGENTS.md`.
   - **Planck EZ:** it is edited only in the Oryx configurator (<https://configure.zsa.io/planck-ez/layouts/BqKMO/latest/0>). Give the user the key code and modifier checkboxes. After a Planck change, update the Obsidian reference per `../PlankEZ-Raw-Source/AGENTS.md`.
   - **Receiver:** the user records the hotkey in Raycast (or the target app). This skill cannot write Raycast settings.

## Choosing a macro chord

Take the first free pool `free --style macro` lists: bare F13–F20, then Hyper or Ctrl+Opt+Cmd with F13–F20, then Meh-like with F13–F20, then F1–F12, then punctuation and navigation keys. No keyboard has a physical F13+, so those chords never collide with typing. Leave Hyper+letter and Hyper+digit for human chords; that space is small and mnemonic.

## Choosing a human chord

Hyper is one key: holding Esc on the K2 HE sends Hyper (`EB_ESC_HYPR`), and Karabiner does the same on other keyboards. So:

1. Hyper + the mnemonic letter (first letter of the app or action).
2. Prefer right-hand keys (Y–P, H–;, N–/, 6–0). Esc is held by the left hand.
3. Then Hyper + digit or punctuation; Ctrl+Opt+Cmd + letter last (a three-key hold).
4. Use no F-keys: the Planck EZ has no F-row.

## Source notes

- **Raycast:** the newest `.rayconfig` in `~/Documents/Library/R/Raycast Exports` by mtime, or `--export PATH`. The passphrase comes from the 1Password item "Raycast Backup/ Export Password" (override with `RAYCAST_BACKUP_PASSWORD`). Decryption happens in memory; the decrypted JSON holds AI chats and clipboard history, so it stays out of files and output. Only enabled commands count.
- **K2 HE:** read from the `k2_he_2025q3` git ref (`--k2he-ref` overrides), so uncommitted edits in `../k2_he` are invisible. A `#define` not placed on a key still counts as taken because it records intent.
- **Planck EZ:** fetched live from the Oryx GraphQL API (layout `BqKMO`, latest revision), all layers, including tap, hold, and double-tap.
- **macOS:** enabled `com.apple.symbolichotkeys` entries plus the script's `RESERVED` list. The plist omits defaults the user never changed; when you find another built-in chord, add it to `RESERVED`.
- **Not covered:** apps that own their own global hotkeys (dictation, AeroSpace when it is running). When the user names one, check it by hand.
