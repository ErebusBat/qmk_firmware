/* Copyright 2025 @ ErebusBat
 *
 * System76 Launch-specific keycodes. Shared chord defines (EB_*, TTS_*) live in
 * ../common/eb_keys.h (ErebusBat/qmk-common); rules.mk adds it to the include path.
 *
 * Reference Links:
 *  - https://docs.qmk.fm/feature_layers
 */

#pragma once

#include "eb_keys.h"

////////////////////////////////////////////////////////////////////////////////
// Start original sys76 custom keys
////////////////////////////////////////////////////////////////////////////////
// Windowing shortcuts (legacy Raycast/Rectangle bindings for this board)
// #define EB_WM_QTR_A LCA(KC_U)
// #define EB_WM_QTR_B LCA(KC_I)
// #define EB_WM_23RD_A LCA(KC_D)
// #define EB_WM_23RD_B LCA(KC_F)
// #define EB_WM_23RD_C LCA(KC_G)
// #define EB_WM_6TH_A HYPR(KC_7)
// #define EB_WM_6TH_B HYPR(KC_8)
// #define EB_WM_6TH_C HYPR(KC_9)
//
// #define EB_WM_QTR_C LCA(KC_J)
// #define EB_WM_QTR_D LCA(KC_K)
// #define EB_WM_HLF_A LCA(KC_E)
// #define EB_WM_HLF_B LCAG(KC_H)
// #define EB_WM_FULLS LCA(KC_T)
// #define EB_WM_6TH_D HYPR(KC_U)
// #define EB_WM_6TH_E HYPR(KC_I)
// #define EB_WM_6TH_F HYPR(KC_O)
//
// #define EB_WM_23RD_R LCAG(KC_EQL)
// #define EB_WM_3RD_A LCAG(KC_F10)
// #define EB_WM_6TH_T LCAG(KC_F2)
// #define EB_WM_3RD_B LCAG(KC_F11)
// #define EB_WM_3RD_C LCAG(KC_F12)
//
// #define EB_WM_6TH_L LCAG(KC_F1)
// #define EB_WM_CENTER LCA(KC_ENT)
//
// #define EB_WM_MOVE_LEFT LCA(KC_LEFT)
// #define EB_WM_MOVE_LEFT_WIDE LCAG(KC_LEFT)
// #define EB_WM_MOVE_RIGHT LCA(KC_RGHT)
