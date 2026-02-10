# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Repository Overview

This is ErebusBat's fork of the QMK firmware repository with custom keymaps for multiple keyboards. It tracks three upstreams:
- **keychron** — `git@github.com:Keychron/qmk_firmware.git` (Keychron boards)
- **sys76** — `git@github.com:system76/qmk_firmware.git` (System76 Launch boards)
- **origin** — `git@github.com:ErebusBat/qmk_firmware.git` (this fork, originally forked from sys76)

## Git: Always use Homebrew git

```bash
/opt/homebrew/bin/git
```
The system git has shell integration conflicts. Always use the full path above for all git operations when on macOS.

## Branch Strategy

Each keyboard family lives on its own branch tracking its respective upstream:

| Branch | Upstream | Keyboards |
|---|---|---|
| `playground` | keychron/playground | Keychron C3 Pro |
| `hall_effect_playground` | keychron/hall_effect_playground | Keychron K2 HE |
| `erebusbat-keyboard` | sys76/master | System76 Launch 1 (custom keymap) |
| `master` | sys76/master | System76 Launch 1 (default keymaps only) |

**Always check out the correct branch before working on a keyboard.**

## Cross-Branch Shared Files

The following files are shared across all branches and must be kept in sync:
- `CLAUDE.md` — this file
- `activate.sh` — QMK build environment activation script

After updating a shared file on any branch, sync it to the other branches (`playground`, `hall_effect_playground`, `master`, `erebusbat-keyboard`):

```bash
# From the target branch, pull the file from the source branch:
/opt/homebrew/bin/git checkout <source-branch> -- CLAUDE.md activate.sh
/opt/homebrew/bin/git add -f CLAUDE.md activate.sh
/opt/homebrew/bin/git commit -m "Sync shared files from <source-branch>"
```

## Build Environment Setup (macOS)

```bash
curl -fsSL https://install.qmk.fm | sh
qmk setup -H $HOME/src/erebusbat/qmk_firmware
```

The install script installs the QMK CLI, ARM/AVR toolchains, and flashing utilities to `~/Library/Application Support/qmk`.

### Activating the build environment

The QMK toolchains are not on PATH by default. Before compiling, source the activation script in your current shell:

```bash
source activate.sh
```

This prepends the QMK toolchain paths for the current session only.

## Build Commands

This repo uses the `qmk` CLI. General pattern:
```bash
qmk compile -kb <keyboard> -km <keymap>
qmk flash -kb <keyboard> -km <keymap>
```

### Per-keyboard build commands

**Keychron C3 Pro** (branch: `playground`):
```bash
qmk compile -kb keychron/c3_pro/ansi/red -km erebusbat
qmk flash -kb keychron/c3_pro/ansi/red -km erebusbat
```

**Keychron K2 HE** (branch: `hall_effect_playground`):
```bash
qmk compile -kb keychron/k2_he/ansi_rgb -km erebusbat
qmk flash -kb keychron/k2_he/ansi_rgb -km erebusbat
```

**System76 Launch 1** (branch: `erebusbat-keyboard`):
```bash
qmk compile -kb system76/launch_1 -km erebusbat
qmk flash -kb system76/launch_1 -km erebusbat
```

### DFU Mode

To flash, the keyboard must be in DFU mode. Hold **Esc** while plugging in the USB cable.

### Hardware Reset (K2 HE only)

If the keyboard firmware is completely corrupt (cannot enter DFU via Esc+plug), there is a **small gold contact pad underneath the spacebar, to the left**. It does not look like a traditional button. Press it down (e.g. with a plastic fork) while plugging in the USB cable to force entry into the STM32 system bootloader. In this deeply broken state the keyboard lights will not work, so confirm DFU mode with:

```bash
dfu-util -l
```

Then flash manually:

```bash
dfu-util -a 0 -d 0483:DF11 -s 0x08000000:leave -D <firmware.bin>
```

### K2 HE EEPROM / Hall Effect Calibration Recovery

**CRITICAL WARNING**: Never call `eeconfig_init()` or `eeconfig_disable()` directly in custom firmware for the K2 HE. The hall effect keyboard stores per-key analog calibration data in EEPROM. Corrupting this data causes a persistent error state (green light flashing under the 4 key on any keypress) that **survives firmware reflashing** because EEPROM persists across flashes.

If the K2 HE enters this error state, the recovery procedure is:

1. Put keyboard in DFU mode (Esc+plug or hardware reset pad under spacebar — see above)
2. Flash the stock Keychron `via` keymap from this repo:
   ```bash
   qmk compile -kb keychron/k2_he/ansi_rgb -km via
   dfu-util -a 0 -d 0483:DF11 -s 0x08000000:leave -D keychron_k2_he_ansi_rgb_via.bin
   ```
3. Try Fn+J+Z factory reset (hold 3 seconds until red flash)
4. If still broken, use **Keychron Launcher** web app (https://launcher.keychron.com) to connect and reset
5. Use Keychron Launcher's firmware upgrade feature to flash official stock firmware
6. After the keyboard is fully recovered with stock firmware, you can flash the custom `erebusbat` keymap again

**Keychron official firmware downloads**: https://www.keychron.com/pages/firmware-and-json-files-of-the-keychron-he-series-keyboards

**Incident reference**: Commit `29296632f2` (VIA enable) triggered the issue when `eeconfig_init()` was called during troubleshooting, corrupting hall effect calibration. Reverted in `5eb29de2ef`. Recovery required flashing the stock `via` keymap compiled at commit `5eb29de2ef` on `hall_effect_playground` (Keychron upstream source from merge `9d61696df8`), then using Keychron Launcher web app firmware upgrade to fully restore EEPROM state.

## ErebusBat Keymap Architecture

All custom keymaps live under `keyboards/<vendor>/<board>/.../keymaps/erebusbat/` and share common conventions.

### Standard files per keymap
- `keymap.c` — layer definitions and `process_record_user`
- `erebusbat.h` — shared custom keycode defines (currently duplicated per keymap)
- `config.h` — mod-tap timing overrides

### Layer convention (Keychron boards)
All Keychron keymaps use 4 layers:
- `MAC_BASE` (0) — default Mac layout, always set as default layer
- `MAC_FN` (1) — Mac function layer, activated via `MO(MAC_FN)`
- `WIN_BASE` (2) — on C3 Pro this is repurposed as a **Windowing layer** (not Windows OS)
- `WIN_FN` (3) — Windows function layer

### Custom keycodes (`EB_` prefix)
Defined in `erebusbat.h` in each keymap directory:
- `EB_CAPS` — `MT(MOD_LCTL, KC_ESC)`: tap for Escape, hold for Left Control
- `EB_ESC_HYPR` — `MT(MOD_HYPR, KC_ESC)`: tap for Escape, hold for Hyper (Ctrl+Shift+Alt+Gui)
- `EB_SCRN` — custom keycode that sends `Cmd+Shift+4` (macOS screenshot region)

### Windowing macros (Raycast integration)
The `EB_` windowing defines use `LCAG()` (Left Ctrl+Alt+Gui) modifier combos bound to Raycast window management on macOS:
- `EB_FULLS` — fullscreen
- `EB_HLF_A/B` — left/right half
- `EB_3RD_A/B/C` — thirds
- `EB_23RD_A/B/C` — two-thirds
- `EB_QTR_A/B/C/D` — quarters (2x2 grid)
- `EB_6th_A-F` — sixths (2x3 grid)

### Mod-tap settings
Configured in each keymap's `config.h`:
- `TAPPING_TERM 200` — hold/tap threshold
- `PERMISSIVE_HOLD` — register hold immediately when another key is pressed
- `HOLD_ON_OTHER_KEY_PRESS` — complementary hold behavior

### MAC_FN layer patterns
The Fn layer maps vim-style arrows to HJKL and wraps bottom-row keys in `C()` (Control) for Ctrl+modifier combos on the left hand side.

## Code Style

- Use `// clang-format off` / `// clang-format on` around keymap matrix definitions to preserve alignment
- Keycode alignment in keymap matrices matters for readability — keep columns aligned
- Use `_______` for transparent keys (standard QMK convention)
- Custom keycodes use `EB_` prefix
- Copyright header: `Copyright 2025 @ ErebusBat`
