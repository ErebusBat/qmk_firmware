/* Copyright 2025 @ ErebusBat */

#ifdef LED_MATRIX_KEYREACTIVE_ENABLED

// Solid base at val/2; on keypress, pulses up to full val then fades back.
LED_MATRIX_EFFECT(SOLID_REACTIVE_OVERLAY)
#    ifdef LED_MATRIX_CUSTOM_EFFECT_IMPLS

static uint8_t SOLID_REACTIVE_OVERLAY_math(uint8_t val, uint16_t offset) {
    uint8_t base  = val >> 1;
    uint8_t boost = scale8(qsub8(255, (uint8_t)MIN(255, offset)), base);
    return qadd8(base, boost);
}

static bool SOLID_REACTIVE_OVERLAY(effect_params_t* params) {
    return effect_runner_reactive(params, &SOLID_REACTIVE_OVERLAY_math);
}

#    endif // LED_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // LED_MATRIX_KEYREACTIVE_ENABLED
