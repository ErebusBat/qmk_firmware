# Erebusbat Keymap for Keychron K2 HE

See `keyboards/keychron/k2_he/CLAUDE.md` for K2 HE-specific docs (DFU, hardware reset, EEPROM recovery, porting notes).

## Overview
Custom QMK keymap for Keychron K2 HE (Hall Effect) ANSI keyboard with 4 layers defined in `keymap.c`:
- MAC_BASE: Default Mac layout
- MAC_FN: Mac function layer
- WIN_BASE: Default Windows layout
- WIN_FN: Windows function layer

## Custom Keycodes
See definitions in `erebusbat.h` in this directory:
- `EB_CAPS`: Defined at erebusbat.h:9
- `EB_ESC_HYPR`: Defined at erebusbat.h:10

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
├── erebusbat.h # Custom keycode definitions
└── CLAUDE.md   # This documentation file
```
