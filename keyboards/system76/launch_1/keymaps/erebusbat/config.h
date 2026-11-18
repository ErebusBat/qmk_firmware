/* Copyright 2026 @ ErebusBat
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#pragma once

#define TAPPING_TERM 200

// Set to 1 and rebuild to try the K2 HE overlap rules on the Caps-position key.
#define EB_K2HE_CAPS_ESC 0

#if EB_K2HE_CAPS_ESC
#    define PERMISSIVE_HOLD_PER_KEY
#    define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#    define QUICK_TAP_TERM_PER_KEY
#    define RETRO_TAPPING_PER_KEY
#else
#    define PERMISSIVE_HOLD
#    define HOLD_ON_OTHER_KEY_PRESS
#endif
