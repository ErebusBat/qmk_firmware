# C3 Pro Splash Effect — Debugging Notes

Attempted to upgrade `SOLID_REACTIVE_OVERLAY` from `effect_runner_reactive` to `effect_runner_reactive_splash`. Compiled and flashed fine but produced no visible LED output. Reverted to working `effect_runner_reactive` version.

**Root cause of "no LEDs" was unrelated to splash**: stale EEPROM was overriding compiled defaults. Fixed by adding `_noeeprom` calls in `keyboard_post_init_user`:
```c
led_matrix_enable_noeeprom();
led_matrix_mode_noeeprom(LED_MATRIX_CUSTOM_SOLID_REACTIVE_OVERLAY);
led_matrix_set_val_noeeprom(128);
```

**Splash runner signature** (for when we revisit):
```c
// math fn:
static uint8_t fn(uint8_t val, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick)
// runner call:
effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &fn);
```

Wave math: `uint16_t effect = tick - dist` — underflows when wave hasn't arrived, peaks near 0 at wavefront, grows as wave passes. Needs further debugging to determine why splash produced no output despite compiling cleanly.
