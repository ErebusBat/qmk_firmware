/* Copyright 2025 @ ErebusBat
 *
 * C3 Pro-specific keycodes. Shared chord defines (EB_*, TTS_*) live in
 * ../common/eb_keys.h (ErebusBat/qmk-common); rules.mk adds it to the include path.
 *
 * Reference Links:
 *  - https://docs.qmk.fm/feature_layers
 */

#pragma once

#include "eb_keys.h"

////////////////////////////////////////////////////////////////////////////////
/// Tap Dance
////////////////////////////////////////////////////////////////////////////////
enum {
    TD_DICT,
};
#define EB_DICT TD(TD_DICT)
