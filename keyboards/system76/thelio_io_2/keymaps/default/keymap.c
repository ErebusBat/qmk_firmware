#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT(QK_BOOT),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}
