/* Copyright 2024 @ Keychron (https://www.keychron.com)
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

#include QMK_KEYBOARD_H
#include "keychron_common.h"
#include "erebusbat.h"

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(WIN_FN)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*

     # MAC_BASE
     # effective_min_key_width=8 token_min_key_width=11
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      | Esc/◆  |   F1   |   F2   |   F3   |   F4   |   F5   |   F6   |   F7   |   F8   |   F9   |  F10   |  F11   |  F12   |  SNAP  |🦇AI_NXT| 🦇DICT |
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+
      |   ~    |   1    |   2    |   3    |   4    |   5    |   6    |   7    |   8    |   9    |   0    |   -    |   =+   |      BSPC       |  PGUP  |
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+
      +-------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+------------+--------+
      |     TAB     |   Q    |   W    |   E    |   R    |   T    |   Y    |   U    |   I    |   O    |   P    |  LBRC  |  RBRC  |    BSLS    |  PGDN  |
      +-------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+------------+--------+
      +---------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-------------------+--------+
      |   Esc/Ctrl    |   A    |   S    |   D    |   F    |   G    |   H    |   J    |   K    |   L    |   ;:   |   '"   |        ENT        |  HOME  |
      +---------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-------------------+--------+
      +-------------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+---------------+--------+--------+
      |       LSFT        |   Z    |   X    |   C    |   V    |   B    |   N    |   M    |   ,<   |   .>   |   /?   |     RSFT      |   UP   |  END   |
      +-------------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+---------------+--------+--------+
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
      |  FN_MAC  |  LOPTN   ||  LCMMD   |                          SPC                          | RCMMD  | FN_MAC |  RCTL  |  LEFT  |  DOWN  |  RGHT  |
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
*/

    [MAC_BASE] = LAYOUT_ansi_84(

     // EB_ESC_HYPR     F1           F2           F3           F4           F5           F6           F7           F8           F9           F10          F11          F12          SNAP      EB_AI_NXT    EB_DICT
        EB_ESC_HYPR, KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       KC_F9,       KC_F10,      KC_F11,      KC_F12,      KC_SNAP,     EB_AI_NXT,   EB_DICT,

     // ~               1            2            3            4            5            6            7            8            9            0         -            =+              BSPC                      PGUP
        KC_GRV,      KC_1,        KC_2,        KC_3,        KC_4,        KC_5,        KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,                  KC_PGUP,

     //    TAB                 Q            W            E            R            T            Y            U            I            O            P            LBRC         RBRC         BSLS               PGDN
        KC_TAB,             KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,        KC_LBRC,     KC_RBRC,     KC_BSLS,           KC_PGDN,

     // EB_CAPS                   A            S            D            F            G            H            J            K            L         ;:           '"              ENT                          HOME
        EB_CAPS,               KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,     KC_ENT,                      KC_HOME,

     //    LSFT                         Z            X            C            V            B            N            M         ,<           .>           /?              RSFT                   UP           END
        KC_LSFT,                     KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     KC_RSFT,               KC_UP,       KC_END,

     // FN_MAC             LOPTN            LCMMD           SPC                                                                              RCMMD     FN_MAC          RCTL         LEFT         DOWN         RGHT
        FN_MAC,         KC_LOPTN,        KC_LCMMD,       KC_SPC,                                                                          KC_RCMMD,    FN_MAC,      KC_RCTL,     KC_LEFT,     KC_DOWN,     KC_RGHT),

/*

     # MAC_FN
     # effective_min_key_width=8 token_min_key_width=9
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      |        |  BRID  |  BRIU  | MCTRL  | LNPAD  |UG_VALD |UG_VALU |  PREV  |  PLAY  |  NEXT  |  MUTE  |  VOL-  |  VOL+  |        |  DEL   |UG_NEXT |
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+
      |        |BT_HST1 |BT_HST2 |BT_HST3 | P2P4G  |        |        |        |        |        |        |        |        |       DEL       |🦇AI_PRV|
      +--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+
      +-------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+------------+--------+
      |   UG_TOGG   |UG_NEXT |UG_VALU |UG_HUEU |UG_SATU |UG_SPDU |        |        |        |        |        |        |        |            |🦇AI_NXT|
      +-------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+------------+--------+
      +---------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-------------------+--------+
      |               |UG_PREV |UG_VALD |UG_HUED |UG_SATD |UG_SPDD |  LEFT  |  DOWN  |   UP   |  RGHT  |        |        |     🦇TUI_NL      |        |
      +---------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-------------------+--------+
      +-------------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+---------------+--------+--------+
      |    C(KC_LSFT)     |C(KC_Z) |C(KC_X) |C(KC_C) |C(KC_V) |BAT_LVL |NK_TOGG |        |        |        |        |               |  VOL+  |  MUTE  |
      +-------------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+---------------+--------+--------+
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
      |          |C(KC_LALT)||C(KC_LGUI)|                       C(KC_SPC)                       |        |        |        |  PREV  |  VOL-  |  NEXT  |
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
*/

    [MAC_FN] = LAYOUT_ansi_84(

     // EB_ESC_HYPR   F1         F2         F3         F4         F5         F6         F7         F8         F9         F10        F11        F12        SNAP    EB_AI_NXT  EB_DICT
        _______,   KC_BRID,   KC_BRIU,   KC_MCTRL,  KC_LNPAD,  UG_VALD,   UG_VALU,   KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_MUTE,   KC_VOLD,   KC_VOLU,   _______,   KC_DEL,    UG_NEXT,

     // ~             1          2          3          4          5          6          7          8          9          0       -          =+            BSPC                  PGUP
        _______,   BT_HST1,   BT_HST2,   BT_HST3,   P2P4G,     _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_DEL,               EB_AI_PRV,

     //    TAB              Q          W          E          R          T          Y          U          I          O          P          LBRC       RBRC       BSLS            PGDN
        UG_TOGG,         UG_NEXT,   UG_VALU,   UG_HUEU,   UG_SATU,   UG_SPDU,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,        EB_AI_NXT,

     // EB_CAPS               A          S          D          F          G          H          J          K          L       ;:         '"            ENT                      HOME
        _______,           UG_PREV,   UG_VALD,   UG_HUED,   UG_SATD,   UG_SPDD,   KC_LEFT,   KC_DOWN,   KC_UP,     KC_RGHT,   _______,   _______,   EB_TUI_NL,               _______,

     //    LSFT                     Z          X          C          V          B          N          M       ,<         .>         /?            RSFT               UP         END
        C(KC_LSFT),              C(KC_Z),   C(KC_X),   C(KC_C),   C(KC_V),   BAT_LVL,   NK_TOGG,   _______,   _______,   _______,   _______,   _______,           KC_VOLU,   KC_MUTE,

     // FN_MAC           LOPTN         LCMMD        SPC                                                                  RCMMD   FN_MAC        RCTL       LEFT       DOWN       RGHT
        _______,      C(KC_LALT),   C(KC_LGUI),  C(KC_SPC),                                                           _______,   _______,   _______,   KC_MPRV,   KC_VOLD,   KC_MNXT),

/*

     # WIN_BASE
     # effective_min_key_width=7 token_min_key_width=11
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
      | Esc/◆ |  F1   |  F2   |  F3   |  F4   |  F5   |  F6   |  F7   |  F8   |  F9   |  F10  |  F11  |  F12  | PSCR  |  DEL  |UG_NEXT|
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-------+
      |   ~   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   0   |   -   |  =+   |     BSPC      | PGUP  |
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-------+
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-------+
      |    TAB    |   Q   |   W   |   E   |   R   |   T   |   Y   |   U   |   I   |   O   |   P   | LBRC  | RBRC  |   BSLS    | PGDN  |
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-------+
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-------+
      |    CAPS     |   A   |   S   |   D   |   F   |   G   |   H   |   J   |   K   |   L   |  ;:   |  '"   |       ENT       | HOME  |
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-------+
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-------+
      |      LSFT       |   Z   |   X   |   C   |   V   |   B   |   N   |   M   |  ,<   |  .>   |  /?   |    RSFT     |  UP   |  END  |
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-------+
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-------+
      |  LCTL   |  LGUI   |  LALT   |                       SPC                       | RALT  |FN_WIN | RCTL  | LEFT  | DOWN  | RGHT  |
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-------+
*/

    [WIN_BASE] = LAYOUT_ansi_84(

     // EB_ESC_HYPR     F1           F2           F3           F4           F5           F6           F7           F8           F9           F10          F11          F12          SNAP      EB_AI_NXT    EB_DICT
        EB_ESC_HYPR, KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       KC_F9,       KC_F10,      KC_F11,      KC_F12,      KC_PSCR,     KC_DEL,      UG_NEXT,

     // ~               1            2            3            4            5            6            7            8            9            0         -            =+              BSPC                      PGUP
        KC_GRV,      KC_1,        KC_2,        KC_3,        KC_4,        KC_5,        KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,                  KC_PGUP,

     //    TAB                 Q            W            E            R            T            Y            U            I            O            P            LBRC         RBRC         BSLS               PGDN
        KC_TAB,             KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,        KC_LBRC,     KC_RBRC,     KC_BSLS,           KC_PGDN,

     // EB_CAPS                   A            S            D            F            G            H            J            K            L         ;:           '"              ENT                          HOME
        KC_CAPS,               KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,     KC_ENT,                      KC_HOME,

     //    LSFT                         Z            X            C            V            B            N            M         ,<           .>           /?              RSFT                   UP           END
        KC_LSFT,                     KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     KC_RSFT,               KC_UP,       KC_END,

     // FN_MAC             LOPTN            LCMMD           SPC                                                                              RCMMD     FN_MAC          RCTL         LEFT         DOWN         RGHT
        KC_LCTL,        KC_LGUI,         KC_LALT,        KC_SPC,                                                                          KC_RALT,     FN_WIN,      KC_RCTL,     KC_LEFT,     KC_DOWN,     KC_RGHT),

/*

     # WIN_FN
     # effective_min_key_width=7 token_min_key_width=7
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
      |       | BRID  | BRIU  | TASK  | FILE  |UG_VALD|UG_VALU| PREV  | PLAY  | NEXT  | MUTE  | VOL-  | VOL+  |       |       |UG_TOGG|
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-------+
      |       |BT_HST1|BT_HST2|BT_HST3| P2P4G |       |       |       |       |       |       |       |       |               |       |
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-------+
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-------+
      |  UG_TOGG  |UG_NEXT|UG_VALU|UG_HUEU|UG_SATU|UG_SPDU|       |       |       |       |       |       |       |           |       |
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-------+
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-------+
      |             |UG_PREV|UG_VALD|UG_HUED|UG_SATD|UG_SPDD|       |       |       |       |       |       |                 |       |
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-------+
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-------+
      |                 |       |       |       |       |BAT_LVL|NK_TOGG|       |       |       |       |             |       |       |
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-------+
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-------+
      |         |         |         |                                                 |       |       |       |       |       |       |
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-------+
*/

    [WIN_FN] = LAYOUT_ansi_84(

     // EB_ESC_HYPR   F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      SNAP  EB_AI_NXTEB_DICT
        _______, KC_BRID, KC_BRIU, KC_TASK, KC_FILE, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, UG_TOGG,

     // ~           1        2        3        4        5        6        7        8        9        0     -        =+          BSPC              PGUP
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,

     //    TAB           Q        W        E        R        T        Y        U        I        O        P        LBRC     RBRC     BSLS         PGDN
        UG_TOGG,      UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______,     _______,

     // EB_CAPS            A        S        D        F        G        H        J        K        L     ;:       '"          ENT                 HOME
        _______,        UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, _______, _______, _______, _______,            _______,

     //    LSFT                Z        X        C        V        B        N        M     ,<       .>       /?          RSFT            UP       END
        _______,            _______, _______, _______, _______, BAT_LVL, NK_TOGG, _______, _______, _______, _______, _______,        _______, _______,

     // FN_MAC        LOPTN       LCMMD      SPC                                                     RCMMD FN_MAC      RCTL     LEFT     DOWN     RGHT
        _______,   _______,    _______,   _______,                                                _______, _______, _______, _______, _______, _______),
};

// clang-format on

// Tap Dance: EB_DICT — tap: HYPR+\, double-tap: Enter
tap_dance_action_t tap_dance_actions[] = {
    [TD_DICT] = ACTION_TAP_DANCE_DOUBLE(EB_KC_DICT, KC_ENT),
};

// Key override: Shift + EB_AI_NXT sends EB_AI_PRV (global across layers).
// Requires KEY_OVERRIDE_ENABLE = yes in rules.mk.
const key_override_t  ai_prev_on_shift = ko_make_basic(MOD_MASK_SHIFT, EB_AI_NXT, EB_AI_PRV);
const key_override_t  tui_nl_on_shift  = ko_make_basic(MOD_MASK_SHIFT, EB_DICT, EB_TUI_NL);
const key_override_t *key_overrides[]  = {
    &ai_prev_on_shift,
    &tui_nl_on_shift,
    NULL, // Terminator required by QMK to mark end of overrides list.
};

// Keep an unchorded Caps-position press as Escape even when it exceeds TAPPING_TERM.
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    return keycode == EB_CAPS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    return true;
}
