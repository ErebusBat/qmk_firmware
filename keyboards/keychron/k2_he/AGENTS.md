# Keychron K2 HE (Hall Effect)

**Branch**: `k2_he_2025q3` (based on Keychron's `2025q3` branch)
**Build**: `qmk compile -kb keychron/k2_he/ansi -km erebusbat`
**Flash**: `qmk flash -kb keychron/k2_he/ansi -km erebusbat`

## Branch History

The K2 HE was originally developed on Keychron's `hall_effect_playground` branch (v1.1.0 level). That branch has an incompatible EEPROM layout with the official v1.2.x firmware. The `2025q3` branch contains the updated v1.2.x codebase with per-key RGB, mixed RGB modes, and the correct EEPROM layout.

| Branch | K2 HE added | Firmware level | Status |
|---|---|---|---|
| `hall_effect_playground` | Oct 8, 2024 | v1.1.0 | **Incompatible** with v1.2.x keyboards |
| `2025q3` | Nov 21, 2025 | v1.2.x | **Current** — use this branch |

### Key differences between branches

| Feature | `hall_effect_playground` | `2025q3` |
|---|---|---|
| Keyboard path | `keychron/k2_he/ansi_rgb` | `keychron/k2_he/ansi` |
| Per-key RGB | No | Yes |
| Mixed RGB regions | No | Yes |
| Caps Lock indicator | No | Yes |
| EEPROM layout | Simple | New `eeconfig_kb.h` (language, custom RGB, wireless, debounce) |
| RGB keycodes | `RGB_TOG`, `RGB_MOD`, etc. | `UG_TOGG`, `UG_NEXT`, etc. |
| VIA keymap | `via` | Renamed to `keychron` |

## IMPORTANT: Firmware Version Incompatibility

**The Keychron `hall_effect_playground` branch does NOT contain the latest K2 HE firmware.** The official released firmware (v1.2.x) has features not present on that branch:

| Version | Date | Features |
|---|---|---|
| v1.1.0 | Oct 31, 2024 | Initial release (matches `hall_effect_playground` branch) |
| v1.2.0 | May 24, 2025 | Per-key RGB / Mixed RGB modes, Caps Lock indicator, backlight off / matrix scan off / BT timeout options |
| v1.2.1 | Jun 17, 2025 | Bug fixes for per-key/mixed RGB, factory reset backlight fix |

**Flashing ANY firmware compiled from `hall_effect_playground` causes the flashing-4 error** because the v1.2.x EEPROM layout is incompatible with the older codebase. This is not a bug in the erebusbat keymap — even the stock `default` keymap from `hall_effect_playground` triggers it.

The `2025q3` branch has the correct EEPROM layout and should produce compatible firmware.

## Recovery: Official Firmware v1.2.1

If the keyboard enters the flashing-4 error state, flash the official Keychron v1.2.1 firmware directly via DFU:

1. Download firmware from: https://www.keychron.com/pages/firmware-and-json-files-of-the-keychron-he-series-keyboards
   - File: `k2_he_ansi_rgb_v1.2.1_2506171010.bin` (129,836 bytes)
2. Put keyboard in DFU mode (Esc+plug, or hardware reset pad — see below)
3. Flash:
   ```bash
   dfu-util -a 0 -d 0483:DF11 -s 0x08000000:leave -D k2_he_ansi_rgb_v1.2.1_2506171010.bin
   ```

**Alternative recovery** (if direct DFU flash doesn't work or download is unavailable):
1. Flash the stock `via` keymap compiled from the `hall_effect_playground` branch at commit `5eb29de2ef`
2. Use **Keychron Launcher** web app (https://launcher.keychron.com) to connect and upgrade firmware

## DFU Mode

Hold **Esc** while plugging in the USB cable.

### Hardware Reset Pad

If the keyboard cannot enter DFU mode via Esc+plug (e.g. after corrupt firmware), there is a **small gold contact pad underneath the spacebar, to the left**. It does not look like a traditional button. Press it down (e.g. with a plastic fork) while plugging in the USB cable to force entry into the STM32 system bootloader. In this deeply broken state the keyboard lights will not work, so confirm DFU mode with:

```bash
dfu-util -l
```

Then flash manually:

```bash
dfu-util -a 0 -d 0483:DF11 -s 0x08000000:leave -D <firmware.bin>
```

## EEPROM / Hall Effect Calibration

**CRITICAL WARNING**: Never call `eeconfig_init()` or `eeconfig_disable()` directly in custom firmware for the K2 HE. The hall effect keyboard stores per-key analog calibration data in EEPROM. Corrupting this data causes a persistent error state (green light flashing under the 4 key on any keypress) that **survives firmware reflashing** because EEPROM persists across flashes.

## VIA Support

**Status**: Not currently enabled. VIA was tested on the `hall_effect_playground` branch in commit `29296632f2` and reverted in `5eb29de2ef`. On the `2025q3` branch, the `keychron` keymap has `VIA_ENABLE = yes` in its rules.mk.

All `EB_` custom keycodes are `#define` macros expanding to standard QMK keycodes (mod-taps, LCAG combos), so VIA understands them natively.

## Porting Notes (hall_effect_playground → 2025q3)

When porting keymaps from the old `hall_effect_playground` branch:

1. **Directory**: `ansi_rgb` → `ansi`
2. **Build command**: `-kb keychron/k2_he/ansi_rgb` → `-kb keychron/k2_he/ansi`
3. **RGB keycodes**:
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

## Incident History

- Commit `29296632f2` on `hall_effect_playground` (VIA enable) was the first attempt; calling `eeconfig_init()` during troubleshooting corrupted EEPROM. Reverted in `5eb29de2ef`.
- Stock `default` keymap from `hall_effect_playground` also triggers the flashing-4 error, confirming this is a codebase-level incompatibility, not an erebusbat keymap issue.
- Recovery firmware commit: `5eb29de2ef` on `hall_effect_playground` (stock `via` keymap allowed Keychron Launcher to connect for firmware upgrade).

## Key Source Files

- `common/analog_matrix/analog_matrix.c` — Hall effect EEPROM calibration code
- `common/eeconfig_kb.h` — EEPROM layout definition (new in 2025q3)
- `common/backlit_indicator.c` — Caps Lock / backlit indicator (new in 2025q3)
- `common/keychron_common.h` — Keychron common features
