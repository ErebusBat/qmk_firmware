/* Copyright 2025 @ ErebusBat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Mod-tap configuration
// See ESC_HERDR_FIX.md in this folder for the reasoning behind these settings.
#define TAPPING_TERM 200    // Default is 200ms, you can try 175-250
#define PERMISSIVE_HOLD     // Resolve to hold when another key is TAPPED (pressed+released) during the term
#define QUICK_TAP_TERM 120  // Tap-then-hold within 120ms repeats the tap keycode
#define RETRO_TAPPING_PER_KEY // Let selected solo mod-taps tap after TAPPING_TERM
// HOLD_ON_OTHER_KEY_PRESS intentionally NOT defined: it stole Esc on fast "Esc-then-key" rolls
// (turning the mod-tap into Hyper/Ctrl), which broke Esc in herdr. See ESC_HERDR_FIX.md.
