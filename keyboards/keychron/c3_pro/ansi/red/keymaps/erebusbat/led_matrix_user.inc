/* Copyright 2025 @ ErebusBat */

#ifdef LED_MATRIX_KEYREACTIVE_ENABLED

// Full brightness base; each keypress dips brightness by 25%, fading back to full.
// Multiple rapid presses on the same key compound (up to 4x = fully dark).
LED_MATRIX_EFFECT(SOLID_REACTIVE_OVERLAY)
#    ifdef LED_MATRIX_CUSTOM_EFFECT_IMPLS

static bool SOLID_REACTIVE_OVERLAY(effect_params_t* params) {
    LED_MATRIX_USE_LIMITS(led_min, led_max);
    uint8_t val   = led_matrix_eeconfig.val;
    uint8_t count = g_last_hit_tracker.count;

    for (uint8_t i = led_min; i < led_max; i++) {
        LED_MATRIX_TEST_LED_FLAGS();
        uint8_t brightness = val;

        for (uint8_t j = 0; j < count; j++) {
            if (g_last_hit_tracker.index[j] == i) {
                uint16_t tick   = scale16by8(g_last_hit_tracker.tick[j], led_matrix_eeconfig.speed);
                uint8_t  offset = (uint8_t)MIN(255, tick);
                uint8_t  dip    = scale8(qsub8(255, offset), val >> 2);  // 25% dip, fades to 0
                brightness      = qsub8(brightness, dip);
            }
        }
        led_matrix_set_value(i, brightness);
    }
    return led_matrix_check_finished_leds(led_max);
}

#    endif // LED_MATRIX_CUSTOM_EFFECT_IMPLS
#endif     // LED_MATRIX_KEYREACTIVE_ENABLED
