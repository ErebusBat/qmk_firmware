/* Copyright 2025 @ ErebusBat
 *
 * Reference Links:
 *  - https://docs.qmk.fm/feature_layers
 */

#pragma once

#define EB_CAPS MT(MOD_LCTL, KC_ESC)
// #define EB_CTRL_L1
#define EB_ESC_HYPR MT(MOD_HYPR, KC_ESC)

////////////////////////////////////////////////////////////////////////////////
/// Windowing Commands
////////////////////////////////////////////////////////////////////////////////

/// - Fullscreen
#define EB_FULLS LCAG(KC_ENT)

/// - Half
/// |---|---|
/// | A |   |
/// |---|---|
///
/// |---|---|
/// |   | B |
/// |---|---|
#define EB_HLF_A LCAG(KC_LEFT)
#define EB_HLF_B LCAG(KC_RGHT)

/// - Thirds
/// |---|---|---|
/// | A | B | C |
/// |---|---|---|
#define EB_3RD_A LCAG(KC_F10)
#define EB_3RD_B LCAG(KC_F11)
#define EB_3RD_C LCAG(KC_F12)

/// - Two Thirds
/// |-----------|
/// |---A---|   |
/// |-----------|
///
/// |-----------|
/// |  |  B  |  |
/// |-----------|
///
/// |-----------|
/// |   |   C   |
/// |-----------|
#define EB_23RD_A LCAG(KC_0)
#define EB_23RD_B LCAG(KC_MINS)
#define EB_23RD_C LCAG(KC_EQL)

/// - Quarters
/// |---|---|
/// | A | B |
/// |---|---|
/// | C | D |
/// |---|---|
#define EB_QTR_A LCAG(KC_U)
#define EB_QTR_B LCAG(KC_I)
#define EB_QTR_C LCAG(KC_J)
#define EB_QTR_D LCAG(KC_K)

/// - Sixths
/// |---|---|---|
/// | A | B | C |
/// |---|---|---|
/// | D | E | F |
/// |---|---|---|
#define EB_6th_A LCAG(KC_F1)
#define EB_6th_B LCAG(KC_F2)
#define EB_6th_C LCAG(KC_F3)
#define EB_6th_D LCAG(KC_1)
#define EB_6th_E LCAG(KC_2)
#define EB_6th_F LCAG(KC_3)
