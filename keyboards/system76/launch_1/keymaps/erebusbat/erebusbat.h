/* Copyright 2025 @ ErebusBat
 *
 * Reference Links:
 *  - https://docs.qmk.fm/feature_layers
 */

#pragma once

#define KC_SNAP     SGUI(KC_4)           // Screenshot Region: Cmd + Shift + 4
                                         // Oddly enough this IS defined in keychron branch

#define EB_LOCK     LGUI(LCTL(KC_Q))     // macOS Lock Screen: Cmd + Ctrl + Q

#define EB_CAPS     MT(MOD_LCTL, KC_ESC) // Tap: Esc, Hold: Left Ctrl
#define EB_ESC_HYPR MT(MOD_HYPR, KC_ESC) // Tap: Esc, Hold: Hyper
#define EB_KC_DICT  HYPR(KC_BSLS)        // Hyper + \     .
#define EB_LLM_DICT HYPR(KC_RBRC)        // Hyper + ]
#define EB_TUI_NL   S(A(KC_ENT))         // TUI NewLine: Shift + Option + Enter
#define EB_AI_NXT   C(S(KC_RGHT))        // AI Next: Ctrl + Shift + Right
#define EB_AI_PRV   C(S(KC_LEFT))        // AI Prev: Ctrl + Shift + Left
                                         // Note: key overrides apply across all layers unless made layer-specific
                                         // Keymaps live in keymap.c (override behavior is defined there)

////////////////////////////////////////////////////////////////////////////////
/// Tap Dance
////////////////////////////////////////////////////////////////////////////////
// enum {
//     TD_DICT,
// };
// #define EB_DICT TD(TD_DICT)

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
#define EB_HLF_A LCAG(KC_LEFT) // Left half
#define EB_HLF_B LCAG(KC_RGHT) // Right half

/// - Thirds (Equal width columns)
/// +───+───+───+
/// | A | B | C |
/// +───+───+───+
#define EB_3RD_A LCAG(KC_F10) // Left third
#define EB_3RD_B LCAG(KC_F11) // Center third
#define EB_3RD_C LCAG(KC_F12) // Right third

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
#define EB_QTR_A LCAG(KC_U) // Top-left
#define EB_QTR_B LCAG(KC_I) // Top-right
#define EB_QTR_C LCAG(KC_J) // Bottom-left
#define EB_QTR_D LCAG(KC_K) // Bottom-right

/// - Sixths (2x3 grid)
/// +───+───+───+
/// | A | B | C |  Top row
/// +───+───+───+
/// | D | E | F |  Bottom row
/// +───+───+───+
#define EB_6TH_A LCAG(KC_F1) // Top-left
#define EB_6TH_B LCAG(KC_F2) // Top-center
#define EB_6TH_C LCAG(KC_F3) // Top-right
#define EB_6TH_D LCAG(KC_1)  // Bottom-left
#define EB_6TH_E LCAG(KC_2)  // Bottom-center
#define EB_6TH_F LCAG(KC_3)  // Bottom-right

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
