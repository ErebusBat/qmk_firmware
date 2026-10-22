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
    DICT_L,
    WIN_FN,
};

#define FN_MAC MO(MAC_FN)
#define FN_WIN MO(DICT_L)

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
      |      Shift/(      |   Z    |   X    |   C    |   V    |   B    |   N    |   M    |   ,<   |   .>   |   /?   |    Shift/)    |   UP   |  END   |
      +-------------------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+---------------+--------+--------+
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
      |  FN_MAC  |  LOPTN   ||  LCMMD   |                          SPC                          | RCMMD  | FN_WIN |  RCTL  |  LEFT  |  DOWN  |  RGHT  |
      +----------+----------++----------+-------------------------------------------------------+--------+--------+--------+--------+--------+--------+
*/

    [MAC_BASE] = LAYOUT_ansi_84(

     // ESC          F1           F2           F3           F4           F5           F6           F7           F8           F9           F10          F11          F12          SCREENSHOT   DEL          LIGHT
        EB_ESC_HYPR, KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       KC_F9,       KC_F10,      KC_F11,      KC_F12,      KC_SNAP,     EB_AI_NXT,   EB_DICT,

     // GRAVE        1            2            3            4            5            6            7            8            9            0            MINUS        EQUAL        BSPC                      PGUP
        KC_GRV,      KC_1,        KC_2,        KC_3,        KC_4,        KC_5,        KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,                  KC_PGUP,

     // TAB                 Q            W            E            R            T            Y            U            I            O            P            LBRC         RBRC         BSLS               PGDN
        KC_TAB,             KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,        KC_LBRC,     KC_RBRC,     KC_BSLS,           KC_PGDN,

     // CAPS                   A            S            D            F            G            H            J            K            L            SCLN         QUOTE        ENTER                        HOME
        EB_CAPS,               KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,     KC_ENT,                      KC_HOME,

     // LSHIFT                       Z            X            C            V            B            N            M            COMMA        DOT          SLASH        RSHIFT                 UP           END
        SC_LSPO,                     KC_Z,        KC_X,        KC_C,        KC_V,        KC_B,        KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     SC_RSPC,               KC_UP,       KC_END,

     // LCTRL           LOPT             LCMD            SPACE                                                                            RCMD         FN           RCTRL        LEFT         DOWN         RIGHT
        FN_MAC,         KC_LOPTN,        KC_LCMMD,       KC_SPC,                                                                          KC_RCMMD,    FN_WIN,      KC_RCTL,     KC_LEFT,     KC_DOWN,     KC_RGHT),

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

     // ESC        F1         F2         F3         F4         F5         F6         F7         F8         F9         F10        F11        F12        SCREENSHOT DEL        LIGHT
        _______,   KC_BRID,   KC_BRIU,   KC_MCTRL,  KC_LNPAD,  UG_VALD,   UG_VALU,   KC_MPRV,   KC_MPLY,   KC_MNXT,   KC_MUTE,   KC_VOLD,   KC_VOLU,   _______,   KC_DEL,    UG_NEXT,

     // GRAVE      1          2          3          4          5          6          7          8          9          0          MINUS      EQUAL      BSPC                  PGUP
        _______,   BT_HST1,   BT_HST2,   BT_HST3,   P2P4G,     _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   KC_DEL,               EB_AI_PRV,

     // TAB              Q          W          E          R          T          Y          U          I          O          P          LBRC       RBRC       BSLS            PGDN
        UG_TOGG,         UG_NEXT,   UG_VALU,   UG_HUEU,   UG_SATU,   UG_SPDU,   _______,   _______,   _______,   _______,   _______,   _______,   _______,   _______,        EB_AI_NXT,

     // CAPS               A          S          D          F          G          H          J          K          L          SCLN       QUOTE      ENTER                    HOME
        _______,           UG_PREV,   UG_VALD,   UG_HUED,   UG_SATD,   UG_SPDD,   KC_LEFT,   KC_DOWN,   KC_UP,     KC_RGHT,   _______,   _______,   EB_TUI_NL,               _______,

     // LSHIFT                   Z          X          C          V          B          N          M          COMMA      DOT        SLASH      RSHIFT             UP         END
        C(KC_LSFT),              C(KC_Z),   C(KC_X),   C(KC_C),   C(KC_V),   BAT_LVL,   NK_TOGG,   _______,   _______,   _______,   _______,   _______,           KC_VOLU,   KC_MUTE,

     // LCTRL         LOPT          LCMD         SPACE                                                                RCMD       FN         RCTRL      LEFT       DOWN       RIGHT
        _______,      C(KC_LALT),   C(KC_LGUI),  C(KC_SPC),                                                           _______,   _______,   _______,   KC_MPRV,   KC_VOLD,   KC_MNXT),

/*

     # DICT_L
     # effective_min_key_width=7 token_min_key_width=11
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+
      | Esc/◆ |  F1   |  F2   |  F3   |  F4   |  F5   |  F6   |  F7   |  F8   |  F9   |  F10  |  F11  |  F12  | PSCR  |  DEL  |  EB_DICT  |
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-----------+
      |   ~   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   0   |   -   |  =+   |     BSPC      |    PGUP   |
      +-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+---------------+-----------+
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-----------+
      |    TAB    |   Q   |   W   |   E   |   R   |   T   |   Y   |   U   |   I   |   O   |   P   | LBRC  | RBRC  |   BSLS    |MYTTS_PAUSE|
      +-----------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------+-----------+
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-----------+
      |    CAPS     |   A   |   S   |   D   |   F   |   G   |   H   |   J   |   K   |   L   |  ;:   |  '"   |  EB_COPY_READ   | MYTTS_READ|
      +-------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-----------------+-----------+
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-----------+
      |     Shift/(     |   Z   |   X   |   C   |   V   |   B   |   N   |   M   |  ,<   |  .>   |  /?   |   Shift/)   |  UP   | MYTTS_STOP|
      +-----------------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------+-------------+-------+-----------+
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-----------+
      |  LCTL   |  LGUI   |  LALT   |                       SPC                       | RALT  |FN_WIN | RCTL  | LEFT  | DOWN  |    RGHT   |
      +---------+---------+---------+-------------------------------------------------+-------+-------+-------+-------+-------+-----------+
*/

    [DICT_L] = LAYOUT_ansi_84(

     // ESC          F1           F2           F3           F4           F5           F6           F7           F8           F9           F10          F11          F12          SCREENSHOT   DEL          LIGHT
        EB_ESC_HYPR, KC_F1,       KC_F2,       KC_F3,       KC_F4,       KC_F5,       KC_F6,       KC_F7,       KC_F8,       KC_F9,       KC_F10,      KC_F11,      KC_F12,      KC_PSCR,     KC_DEL,      EB_DICT,

     // GRAVE        1            2            3            4            5            6            7            8            9            0            MINUS        EQUAL        BSPC                      PGUP
        KC_GRV,      KC_1,        KC_2,        KC_3,        KC_4,        KC_5,        KC_6,        KC_7,        KC_8,        KC_9,        KC_0,        KC_MINS,     KC_EQL,      KC_BSPC,                  KC_PGUP,

     // TAB                 Q            W            E            R            T            Y            U            I            O            P            LBRC         RBRC         BSLS               PGDN
        KC_TAB,             KC_Q,        KC_W,        KC_E,        KC_R,        KC_T,        KC_Y,        KC_U,        KC_I,        KC_O,        KC_P,        KC_LBRC,     KC_RBRC,     KC_BSLS,           MYTTS_PAUSE,

     // CAPS                   A            S            D            F            G            H            J            K            L            SCLN         QUOTE        ENTER                        HOME
        KC_CAPS,               KC_A,        KC_S,        KC_D,        KC_F,        KC_G,        KC_H,        KC_J,        KC_K,        KC_L,        KC_SCLN,     KC_QUOT,     EB_COPY_READ,                MYTTS_READ,

     // LSHIFT                       Z            X            C            V            B            N            M            COMMA        DOT          SLASH        RSHIFT                 UP           END
        SC_LSPO,                     KC_Z,        KC_X,        EB_COPY,     KC_V,        KC_B,        KC_N,        KC_M,        KC_COMM,     KC_DOT,      KC_SLSH,     SC_RSPC,               KC_UP,       MYTTS_STOP,

     // LCTRL           LOPT             LCMD            SPACE                                                                            RCMD         FN           RCTRL        LEFT         DOWN         RIGHT
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

     // ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      SCREENSHOT DEL    LIGHT
        _______, KC_BRID, KC_BRIU, KC_TASK, KC_FILE, UG_VALD, UG_VALU, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE, KC_VOLD, KC_VOLU, _______, _______, UG_TOGG,

     // GRAVE    1        2        3        4        5        6        7        8        9        0        MINUS    EQUAL    BSPC              PGUP
        _______, BT_HST1, BT_HST2, BT_HST3, P2P4G,   _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______,

     // TAB           Q        W        E        R        T        Y        U        I        O        P        LBRC     RBRC     BSLS         PGDN
        UG_TOGG,      UG_NEXT, UG_VALU, UG_HUEU, UG_SATU, UG_SPDU, _______, _______, _______, _______, _______, _______, _______, _______,     _______,

     // CAPS            A        S        D        F        G        H        J        K        L        SCLN     QUOTE    ENTER               HOME
        _______,        UG_PREV, UG_VALD, UG_HUED, UG_SATD, UG_SPDD, _______, _______, _______, _______, _______, _______, _______,            _______,

     // LSHIFT              Z        X        C        V        B        N        M        COMMA    DOT      SLASH    RSHIFT          UP       END
        _______,            _______, _______, _______, _______, BAT_LVL, NK_TOGG, _______, _______, _______, _______, _______,        _______, _______,

     // LCTRL      LOPT        LCMD       SPACE                                                   RCMD     FN       RCTRL    LEFT     DOWN     RIGHT
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

// A fast Caps-position tap often overlaps the next keypress by a few ms. Global
// PERMISSIVE_HOLD resolves those as Ctrl chords and swallows Esc entirely
// (reproduced in tests/tap_hold_configurations/erebusbat_caps). Opt EB_CAPS out:
// its tap then fires on release while the rolled key is deferred and sent after
// Esc. Deliberate holds still resolve to Ctrl via the TAPPING_TERM timeout.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    return keycode != EB_CAPS;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_keychron_common(keycode, record)) {
        return false;
    }
    if (keycode == EB_COPY_READ) {
        if (record->event.pressed) {
            tap_code16(EB_COPY);
            // Allow the application to update the clipboard before reading it.
            wait_ms(250);
            tap_code16(MYTTS_READ);
        }
        return false;
    }
    return true;
}
