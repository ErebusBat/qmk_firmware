# Erebusbat Keymap for Keychron K2 HE

See `keyboards/keychron/k2_he/AGENTS.md` for K2 HE-specific docs (DFU, hardware reset, EEPROM recovery, porting notes).

## Overview
Custom QMK keymap for Keychron K2 HE (Hall Effect) ANSI keyboard with 4 layers defined in `keymap.c`:
- MAC_BASE: Default Mac layout
- MAC_FN: Mac function layer
- DICT_L: Dictation and MyTTS layer
- WIN_FN: Windows function layer

## Keymap Comments
- Comments immediately above each `LAYOUT_ansi_84` array row identify the physical keys in that row. Use the same physical key labels across all layers and align them with the corresponding array entries.
- The ASCII diagram above each layer shows its mapped actions. Update that diagram when bindings change; keep the row comments tied to the physical keys. For example, the Page Down position stays `PGDN` in the row comment and shows `TTS_PAUSE` in the `DICT_L` diagram.

## Custom Keycodes
- Chord defines shared by every board (`EB_CAPS`, `EB_ESC_HYPR`, `EB_DICT_PASTE_ENH`, `TTS_*`, window commands, ...) live in `../common/eb_keys.h` (repo `ErebusBat/qmk-common`). `rules.mk` adds `../common` to the include path.
- `erebusbat.h` in this directory includes it and holds only K2 HE-specific keycodes: the `EB_COPY_READ` custom keycode and the `TD_DICT` tap dance.

## Mod-Tap Configuration
See `config.h` in this directory for current timing settings:
- `TAPPING_TERM`: config.h:21
- `PERMISSIVE_HOLD`: config.h:22
- `HOLD_ON_OTHER_KEY_PRESS`: config.h:23
- `QUICK_TAP_TERM`: config.h:25 (commented out)

## Build Command
```bash
qmk compile -kb keychron/k2_he/ansi -km erebusbat
qmk flash -kb keychron/k2_he/ansi -km erebusbat
```

## Key Features Used in Keymap
- Bluetooth host switching (BT_HST1-3)
- RGB controls (UG_TOGG, UG_NEXT, UG_VALU/VALD, etc.)
- Media controls (KC_MPLY, KC_MNXT, KC_MPRV, KC_VOLU, KC_VOLD, KC_MUTE)
- Battery level indicator (BAT_LVL)
- N-Key Rollover toggle (NK_TOGG)
- P2P4G wireless mode switching

## File Structure
```
/keyboards/keychron/k2_he/ansi/keymaps/erebusbat/
├── keymap.c    # Main keymap definitions
├── config.h    # Mod-tap timing configuration
├── erebusbat.h # K2 HE-specific keycodes; includes ../common/eb_keys.h
├── rules.mk    # Features, plus the ../common include path
└── AGENTS.md   # This documentation file
```
