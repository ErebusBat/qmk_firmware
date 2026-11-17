/* Copyright 2025 @ ErebusBat
 *
 * Wires the erebusbat keymap's EB_CAPS chord logic into the test firmware, the
 * same way keymap.c does. This must be a C file: keyevent_t has a different
 * layout in the C++ test files (enum size), so reading record->event.pressed
 * from test_caps_esc.cpp gives wrong values.
 */

#include "quantum.h"

#define EB_CAPS CTL_T(KC_ESC)
#include "../../../keyboards/keychron/k2_he/ansi/keymaps/erebusbat/eb_caps_chord.h"

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    if (keycode != EB_CAPS) {
        return true;
    }
    return eb_caps_permissive_hold(record);
}

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    eb_caps_pre_process(keycode, record);
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == EB_CAPS) {
        return eb_caps_process_record(record);
    }
    return true;
}
