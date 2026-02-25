/* Copyright 2025 @ ErebusBat */

#ifdef LED_MATRIX_KEYREACTIVE_ENABLED

// Full brightness base; on keypress, goes dark (0) then fades back to full.
// Rapid presses reset the fade timer, keeping the key dark.
LED_MATRIX_EFFECT(SOLID_REACTIVE_OVERLAY)
#    ifdef LED_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t SOLID_REACTIVE_OVERLAY_math(uint8_t val, uint16_t offset) {
    uint8_t clamped = (uint8_t)MIN(255, offset);
    return scale8(clamped, val);  // 0 at press, fades linearly back to val
}

static bool SOLID_REACTIVE_OVERLAY(effect_params_t* params) {
    return effect_runner_reactive(params, &SOLID_REACTIVE_OVERLAY_math);
}

#    endif // LED_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // LED_MATRIX_KEYREACTIVE_ENABLED
