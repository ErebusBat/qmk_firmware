/* Copyright 2025 @ ErebusBat
 *
 * Reference Links:
 *  - https://docs.qmk.fm/feature_layers
 */

#pragma once

#define EB_CAPS MT(MOD_LCTL, KC_ESC)
#define EB_ESC_HYPR MT(MOD_HYPR, KC_ESC)

////////////////////////////////////////////////////////////////////////////////
/// Windowing Commands - Raycast Windowing Integration
/// Letters (A, B, C, etc.) represent window positions on screen
////////////////////////////////////////////////////////////////////////////////

/// - Fullscreen
/// +───────────+
/// |     A     |
/// +───────────+
#define EB_FULLS LCAG(KC_ENT)

/// - Half Screen (Left/Right)
/// +─────+─────+
/// |  A  |  B  |
/// +─────+─────+
#define EB_HLF_A LCAG(KC_LEFT)  // Left half
#define EB_HLF_B LCAG(KC_RGHT)  // Right half

/// - Thirds (Equal width columns)
/// +───+───+───+
/// | A | B | C |
/// +───+───+───+
#define EB_3RD_A LCAG(KC_F10)   // Left third
#define EB_3RD_B LCAG(KC_F11)   // Center third
#define EB_3RD_C LCAG(KC_F12)   // Right third

/// - Two Thirds (Wide columns)
/// +───────+───+
/// |   A   | C |  A = Left 2/3
/// +───────+───+
/// +───+───────+
/// | A |   C   |  C = Right 2/3
/// +───+───────+
/// +─+───────+─+
/// | |   B   | |  B = Center 2/3
/// +─+───────+─+
#define EB_23RD_A LCAG(KC_0)    // Left two-thirds
#define EB_23RD_B LCAG(KC_MINS) // Center two-thirds
#define EB_23RD_C LCAG(KC_EQL)  // Right two-thirds

/// - Quarters (2x2 grid)
/// +───+───+
/// | A | B |  Top row
/// +───+───+
/// | C | D |  Bottom row
/// +───+───+
#define EB_QTR_A LCAG(KC_U)     // Top-left
#define EB_QTR_B LCAG(KC_I)     // Top-right
#define EB_QTR_C LCAG(KC_J)     // Bottom-left
#define EB_QTR_D LCAG(KC_K)     // Bottom-right

/// - Sixths (2x3 grid)
/// +───+───+───+
/// | A | B | C |  Top row
/// +───+───+───+
/// | D | E | F |  Bottom row
/// +───+───+───+
#define EB_6th_A LCAG(KC_F1)    // Top-left
#define EB_6th_B LCAG(KC_F2)    // Top-center
#define EB_6th_C LCAG(KC_F3)    // Top-right
#define EB_6th_D LCAG(KC_1)     // Bottom-left
#define EB_6th_E LCAG(KC_2)     // Bottom-center
#define EB_6th_F LCAG(KC_3)     // Bottom-right
