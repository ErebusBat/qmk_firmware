# K2 HE Firmware: 2025q3 Branch Discovery and Port (Feb 10, 2026)

## Problem
The Keychron K2 HE erebusbat keymap on the `hall_effect_playground` branch caused flashing-4 error on any keyboard running official v1.2.x firmware. Root cause: the GitHub `hall_effect_playground` branch contains v1.1.0-level code with an incompatible EEPROM layout. Even the stock `default` keymap from that branch triggers the error.

## Discovery
Keychron's `2025q3` branch contains the v1.2.x codebase with:
- Per-key RGB (`default_per_key_led[]`, `default_region[]`)
- Mixed RGB modes
- Caps Lock indicator (`backlit_indicator.c/.h`)
- New EEPROM layout (`eeconfig_kb.h` with language, custom RGB, wireless, debounce sections)
- Directory renamed: `ansi_rgb` → `ansi`
- RGB keycodes updated: `RGB_TOG` → `UG_TOGG`, `RGB_MOD` → `UG_NEXT`, etc.
- `via` keymap renamed to `keychron` keymap

K2 HE was added to 2025q3 on Nov 21, 2025 (commit `2170f7767b`).

## Port
Created new local branch `k2_he_2025q3` tracking `keychron/2025q3`. Ported erebusbat keymap:
- New path: `keyboards/keychron/k2_he/ansi/keymaps/erebusbat/`
- Build command: `qmk compile -kb keychron/k2_he/ansi -km erebusbat`
- Updated all RGB keycodes to new UG_* names
- ChibiOS submodules needed updating (`git submodule update --init lib/chibios lib/chibios-contrib`)
- VIA enabled via `rules.mk` (`VIA_ENABLE = yes`) — works on 2025q3
- Tap Dance enabled for EB_DICT keycode (tap: HYPR+\, double-tap: Enter)
- Firmware size: ~103KB with VIA + Tap Dance
- Flashed and verified working on hardware

## Branch Strategy Update
| Branch | Status | Notes |
|---|---|---|
| `k2_he_2025q3` | **Active** | Tracks keychron/2025q3, v1.2.x compatible |
| `hall_effect_playground` | **Deprecated** | v1.1.0, incompatible with v1.2.x keyboards |

## Key Commits on k2_he_2025q3
- `1234116a83` — Initial port of erebusbat keymap to 2025q3
- `78344c75f9` — Enable VIA support
- `8de3f9ed5f` — Add EB_DICT tap dance keycode

## RGB Keycode Mapping (old → new)
- `RGB_TOG` → `UG_TOGG`
- `RGB_MOD` → `UG_NEXT`
- `RGB_RMOD` → `UG_PREV`
- `RGB_VAI` → `UG_VALU`
- `RGB_VAD` → `UG_VALD`
- `RGB_HUI` → `UG_HUEU`
- `RGB_HUD` → `UG_HUED`
- `RGB_SAI` → `UG_SATU`
- `RGB_SAD` → `UG_SATD`
- `RGB_SPI` → `UG_SPDU`
- `RGB_SPD` → `UG_SPDD`
