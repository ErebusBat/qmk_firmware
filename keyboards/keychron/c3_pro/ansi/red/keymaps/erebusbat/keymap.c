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
#include "erebusbat.h"

enum layers {
    MAC_BASE,
    MAC_FN,
    WIN_BASE,
    WIN_FN,
};

#define KC_TASK LGUI(KC_TAB)
#define KC_FLXP LGUI(KC_E)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_tkl_ansi(
        // ESC,                 KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,     KC_F11,     KC_F11,   KC_PSCR,    KC_SCRL,   KC_PAUS,
        EB_ESC_HYPR,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,     KC_F11,     KC_F12,   EB_SCRN,    EB_AI_NXT, EB_DICT,

        // GRV,       KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,    KC_EQL,     KC_BSPC,  KC_INS,     KC_HOME,   KC_PGUP,
        KC_GRV,       KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,    KC_EQL,     KC_BSPC,  KC_INS,     KC_HOME,   KC_PGUP,

        // TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,    KC_RBRC,    KC_BSLS,  KC_DEL,     KC_END,    KC_PGDN,
        KC_TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,    KC_RBRC,    KC_BSLS,  KC_DEL,     KC_END,    KC_PGDN,

        // CAPS,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                KC_ENT,
        EB_CAPS,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                KC_ENT,

        // LSFT,                KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                KC_RSFT,                KC_UP,
        KC_LSFT,                KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                KC_RSFT,                KC_UP,

        // LCTL,      KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,   MO(WIN_FN), KC_APP,     KC_RCTL,  KC_LEFT,     KC_DOWN,  KC_RGHT),
        MO(MAC_FN),   KC_LOPT,  KC_LCMD,                                KC_SPC,                                 KC_RCMD,   MO(MAC_FN), KC_TRNS,    KC_RCTL,  KC_LEFT,     KC_DOWN,  KC_RGHT),

    [MAC_FN] = LAYOUT_tkl_ansi(
        // ESC,                 KC_F1,       KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   KC_PSCR,  KC_SCRL,  KC_PAUS,
        _______,                KC_BRID,     KC_BRIU,  KC_MCTL,  KC_LPAD,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_PSCR,  KC_TRNS,  KC_PAUS,

        // GRV,       KC_1,     KC_2,        KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,     KC_BSPC,  KC_INS,   KC_HOME,  KC_PGUP,
        _______,      _______,  _______,     _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,

        // TAB,       KC_Q,     KC_W,        KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,    KC_BSLS,  KC_DEL,   KC_END,   KC_PGDN,
        BL_TOGG,      BL_STEP,  BL_UP,       _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,  _______,  _______,

        // CAPS,      KC_A,     KC_S,        KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,
        MO(WIN_BASE), _______,  BL_DOWN,     _______,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  _______,  _______,              EB_TUI_NL,

        // LSFT,                KC_Z,        KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,            KC_UP,
        C(KC_LSFT),             C(KC_Z),     C(KC_X),  C(KC_C),  C(KC_V),  _______,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,            _______,

        // LCTL,      KC_LWIN,  KC_LALT,                                   KC_SPC,                                 KC_RALT,  MO(WIN_FN),KC_APP,    KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),
        _______,      C(KC_LOPT),C(KC_LCMD),                               C(KC_SPC),                              _______,  _______,   _______,   _______,  _______,  _______,  _______),

    // Windowing Layer
    [WIN_BASE] = LAYOUT_tkl_ansi(
        // ESC,                 KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,     KC_F11,     KC_F11,   KC_PSCR,    KC_SCRL,  KC_PAUS,
        KC_ESC,                 KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     EB_3RD_A,   EB_3RD_B,   EB_3RD_C, EB_23RD_A, EB_23RD_B,  EB_23RD_C,

        // GRV,       KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,    KC_EQL,     KC_BSPC,  KC_INS,     KC_HOME,  KC_PGUP,
        KC_GRV,       KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     EB_23RD_A, EB_23RD_B,  EB_23RD_C,  KC_BSPC,  EB_6th_A,   EB_6th_B, EB_6th_C,

        // TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,    KC_RBRC,    KC_BSLS,  KC_DEL,     KC_END,   KC_PGDN,
        KC_TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     EB_QTR_A, EB_QTR_B, KC_O,     KC_P,      KC_LBRC,    KC_RBRC,    KC_BSLS,  EB_6th_D,   EB_6th_E, EB_6th_F,

        // CAPS,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                KC_ENT,
        KC_CAPS,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     EB_QTR_C, EB_QTR_D, KC_L,     KC_SCLN,   KC_QUOT,                EB_FULLS,

        // LSFT,                KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                KC_RSFT,              KC_UP,
        KC_LSFT,                KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                KC_RSFT,              KC_UP,

        // LCTL,      KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,   MO(WIN_FN), KC_APP,    KC_RCTL,  KC_LEFT,     KC_DOWN,  KC_RGHT),
        KC_LCTL,      KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,   MO(WIN_FN), KC_APP,    KC_RCTL,  EB_HLF_A,    KC_DOWN,  EB_HLF_B),

    [WIN_FN] = LAYOUT_tkl_ansi(
        // ESC,                 KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,     KC_F10,     KC_F11,     KC_F11,   KC_PSCR,    KC_SCRL,  KC_PAUS,
        _______,                KC_BRID,  KC_BRIU,  KC_TASK,  KC_FLXP,  BL_DOWN,  BL_UP,    KC_MPRV,  KC_MPLY,  KC_MNXT,   KC_MUTE,    KC_VOLD,    KC_VOLU,  _______,    _______,  BL_STEP,

        // GRV,       KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,      KC_MINS,    KC_EQL,     KC_BSPC,  KC_INS,     KC_HOME,  KC_PGUP,
        _______,      _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,    _______,    _______,  _______,    _______,  _______,

        // TAB,       KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,      KC_LBRC,    KC_RBRC,    KC_BSLS,  KC_DEL,     KC_END,   KC_PGDN,
        BL_TOGG,      BL_STEP,  BL_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,    _______,    _______,  _______,    _______,  _______,

        // CAPS,      KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,   KC_QUOT,                KC_ENT,
        _______,      _______,  BL_DOWN,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,   _______,                _______,

        // LSFT,                KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,    KC_SLSH,                KC_RSFT,              KC_UP,
        _______,                _______,  _______,  _______,  _______,  _______,  NK_TOGG,  _______,  _______,  _______,   _______,                _______,              _______,

        // LCTL,      KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,   MO(WIN_FN), KC_APP,     KC_RCTL,  KC_LEFT,     KC_DOWN,  KC_RGHT),
        _______,      _______,  _______,                                _______,                                _______,   _______,    _______,    _______,  _______,     _______,  _______),
};

tap_dance_action_t tap_dance_actions[] = {
    [TD_DICT] = ACTION_TAP_DANCE_DOUBLE(EB_KC_DICT, KC_ENT),
};

void keyboard_post_init_user(void) {
    // Force MAC_BASE as default layer or things will be weird
    default_layer_set(1U << MAC_BASE);

    // Force LED on with our custom effect at startup, without reading or writing EEPROM
    // Change the brightness and speed to our values defined in config.h
    led_matrix_enable_noeeprom();
    led_matrix_mode_noeeprom(LED_MATRIX_CUSTOM_SOLID_REACTIVE_OVERLAY);
    led_matrix_set_val_noeeprom(LED_MATRIX_DEFAULT_VAL );
    led_matrix_set_speed_noeeprom(LED_MATRIX_DEFAULT_SPD);  // slow fade (~1s back to full)
}
