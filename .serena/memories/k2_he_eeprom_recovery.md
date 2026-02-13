# K2 HE EEPROM / Hall Effect Calibration Recovery

## Problem
Keychron K2 HE enters persistent error state: green light flashes under 4 key on any keypress, backlights turn off. Caused by corrupted hall effect per-key calibration data in EEPROM. Error survives firmware reflashing because EEPROM persists across DFU flashes.

## RESOLVED: Firmware Version Incompatibility
The Keychron `hall_effect_playground` branch does NOT contain v1.2.x firmware code. However, the `2025q3` branch DOES — K2 HE was added there on Nov 21, 2025 (commit `2170f7767b`) with per-key RGB, mixed RGB, caps lock indicator, and the correct EEPROM layout. The erebusbat keymap has been ported to a new `k2_he_2025q3` branch tracking `keychron/2025q3` and is working with VIA enabled. See memory `k2_he_2025q3_port.md` for full details.

## Recovery: Official Firmware v1.2.1
Simplest recovery: download `k2_he_ansi_rgb_v1.2.1_2506171010.bin` (129,836 bytes) from https://www.keychron.com/pages/firmware-and-json-files-of-the-keychron-he-series-keyboards and flash via DFU:
```bash
dfu-util -a 0 -d 0483:DF11 -s 0x08000000:leave -D k2_he_ansi_rgb_v1.2.1_2506171010.bin
```

## Original Root Cause (Secondary)
Calling `eeconfig_init()` or `eeconfig_disable()` directly on hall effect keyboards wipes the analog calibration data stored in EEPROM. This happened during VIA troubleshooting (commit `29296632f2` on `hall_effect_playground`, reverted in `5eb29de2ef`).

## CRITICAL WARNING
**Never call `eeconfig_init()` or `eeconfig_disable()` in custom firmware for hall effect Keychron keyboards.**

## Recovery Procedure
1. Flash stock Keychron `via` keymap from repo: `qmk compile -kb keychron/k2_he/ansi_rgb -km via`
2. Try Fn+J+Z factory reset (hold 3 seconds until red flash)
3. If still broken: use **Keychron Launcher** web app (https://launcher.keychron.com) to connect
4. Use Keychron Launcher firmware upgrade to restore official stock firmware
5. Once working on stock firmware, flash custom `erebusbat` keymap again

## Hardware Reset Pad
Small gold contact pad underneath the spacebar, to the left. Does NOT look like a traditional button. Press down (e.g. with a plastic fork) while plugging USB to force STM32 system bootloader entry when software DFU (Esc+plug) doesn't work. Lights won't work in this broken state — confirm DFU mode with `dfu-util -l`.

## Recovery Firmware Commit
Stock `via` keymap compiled at commit `5eb29de2ef` on `hall_effect_playground` (Keychron upstream from merge `9d61696df8`) was recognized by Keychron Launcher web app for firmware upgrade.

## Key Files
- `keyboards/keychron/common/analog_matrix/analog_matrix.c` — EEPROM calibration init at line 584
- `keyboards/keychron/common/factory_test.c` — Fn+J+Z factory reset combo at line 80

## VIA Compatibility
All `EB_` custom keycodes are `#define` macros (mod-taps, LCAG combos) — VIA understands them natively. Enabling VIA itself (`VIA_ENABLE = yes` in `rules.mk`) adds ~3KB overhead and is safe. The corruption was caused by manual `eeconfig_init()` calls during troubleshooting, not by VIA itself.
