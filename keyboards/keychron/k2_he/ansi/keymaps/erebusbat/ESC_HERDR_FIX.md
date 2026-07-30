# ESC reliability fix (herdr / TUI multiplexers)

_Date: 2026-07-30_

## Symptom

After switching to [herdr](https://github.com/herdrdev/herdr) (a Rust terminal multiplexer),
**ESC sometimes did not register**. The same problem was **not** observed under tmux.

## Root cause — two stacked layers

### 1. Firmware (this keymap)

Both keys that produce Esc on the **Mac** base layer are mod-taps, so the Esc byte is emitted on
key **release** (the firmware waits to see whether you're holding for the modifier). That delays
Esc and compresses the gap to the next keystroke.

| Key (Mac base) | Define | Tap | Hold | Source |
| --- | --- | --- | --- | --- |
| Top-left | `EB_ESC_HYPR = MT(MOD_HYPR, KC_ESC)` | Esc | Hyper | `erebusbat.h:10`, `keymap.c:60`/`:148` |
| Caps position | `EB_CAPS = MT(MOD_LCTL, KC_ESC)` | Esc | Left Ctrl | `erebusbat.h:9`, `keymap.c:69` |

> On the **Windows** base layer the caps position is plain `KC_CAPS` (no Esc there), so this only
> affected Mac.

`config.h` previously enabled **both** `PERMISSIVE_HOLD` **and** `HOLD_ON_OTHER_KEY_PRESS`, with
`QUICK_TAP_TERM` commented out. `HOLD_ON_OTHER_KEY_PRESS` resolves a mod-tap to its **hold**
(Hyper/Ctrl) the instant *any* other key goes down within `TAPPING_TERM` (200 ms). So a fast
"Esc, then key" roll dropped Esc and emitted a modifier chord instead.

### 2. Terminal (herdr — upstream, not fixable here)

Terminals must decide whether a closely-spaced `ESC`+byte is a standalone Esc or the start of a
meta/escape sequence. tmux exposes `escape-time` (set low, ~0–10 ms) so it never coalesces —
which is why tmux was fine. herdr exposes **no** escape-time setting (confirmed against
`herdr.dev/docs/configuration` and `.../keyboard`), and its default coalescing window swallowed
the tightly-spaced Esc as a meta prefix.

### How they interact

Because the mod-tap delayed Esc to key-release, Esc landed tightly against the next key and was
much more likely to be coalesced by herdr. Loosening the firmware side (never steal Esc on a fast
roll, let quick taps always be taps) also relieves the herdr symptom.

## What changed

**File:** `config.h`

- **Removed** `HOLD_ON_OTHER_KEY_PRESS` — the primary offender; it turned fast "Esc-then-key"
  rolls into Hyper/Ctrl chords.
- **Kept** `PERMISSIVE_HOLD` — deliberate holds (hold modifier, fully tap another key) still
  resolve to Hyper/Ctrl, so intentional chords keep working.
- **Enabled** `QUICK_TAP_TERM 120` (was commented out) — a quick re-tap of Esc always registers
  as Esc.
- `TAPPING_TERM` left at `200` (lower to ~175 if hold still feels sticky).

These are global tap-hold settings, so the single change fixes **both** `EB_ESC_HYPR` and
`EB_CAPS`.

## What was NOT changed (and why)

- **Keymap / keycodes** (`keymap.c`, `erebusbat.h`): `EB_ESC_HYPR` and `EB_CAPS` remain mod-taps,
  preserving Hyper-on-hold and Ctrl-on-hold. The config change alone is expected to resolve the
  symptom; converting the top-left to a plain `KC_ESC` is the fallback only if it does not.
- **Hall-Effect rapid-trigger profiles** (`ansi/profiles.c`, profiles 1 & 2 = `AKM_RAPID`): a
  secondary suspect, left untouched. The symptom changed with *usage* (adopting herdr), not with a
  profile switch, so rapid trigger is not the primary cause. Revisit only if Esc chatters even
  when typed slowly and in isolation.
- **Debounce** (`k2_he/info.json`, `debounce: 0`, `sym_eager_pk`): unchanged.

## Fallback (only if Esc is still unreliable after flashing)

Make the top-left a plain key (Esc emits on key-**down**, zero tap-hold logic):

- `erebusbat.h:10` / `keymap.c:60` & `:148`: replace `EB_ESC_HYPR` with plain `KC_ESC`.
- `EB_ESC_HYPR` appears to be the only hold-Hyper source; standalone Hyper keycodes
  (`EB_KC_DICT`, `EB_LLM_DICT`) are unaffected. Relocate Hyper-hold to another key if needed.

## How to confirm the layer

Two complementary tests isolate which layer is responsible:

- **Firmware layer** — `just check` (opens Karabiner-EventViewer). Press **Esc**, then **Esc + a
  letter quickly**. This shows what the *OS* receives. If a Hyper/Ctrl chord fires instead of Esc
  → the **firmware mod-tap** is stealing it (what this change targets).
- **Terminal layer** — in a raw shell run `sed -n l` (or `cat -v`; Ctrl-C to exit), both outside
  and inside herdr. Bare Esc shows `\033` / `^[`. If Esc appears alone but the fast "Esc+letter"
  case makes it vanish / become a meta sequence → **terminal-side (herdr) coalescing** (occurs
  after the OS, so `just check` cannot see it).

## Build / flash

Use the repo `justfile` (recipes wrap `qmk` with `KEYBOARD=keychron/k2_he/ansi`,
`KEYMAP=erebusbat` after sourcing `activate.sh`):

```sh
just build    # alias for `just compile`
just flash    # depends on compile, so it builds first; flash from a dedicated tmux pane (AGENTS.md)
```
