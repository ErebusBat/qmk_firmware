/* Copyright 2025 @ ErebusBat
 *
 * Chord detection for EB_CAPS (tap: Esc, hold: Left Ctrl).
 *
 * QMK decides tap vs hold from TAPPING_TERM and release order. That loses
 * quick Ctrl chords (tmux prefix Caps+A) whenever everything is released inside
 * TAPPING_TERM, and always when Caps is released a moment before the letter.
 * This file adds one rule on top: if the first key pressed while Caps is down
 * overlaps Caps for EB_CAPS_OVERLAP_MS, the gesture is a Ctrl chord, whichever
 * key is released first. A short overlap stays an Esc-then-key roll.
 *
 * Included by keymap.c and by tests/tap_hold_configurations/erebusbat_caps, so
 * the tests run this exact code. EB_CAPS must be defined before including.
 * See ESC_HERDR_FIX.md.
 */

#pragma once

#ifndef EB_CAPS_OVERLAP_MS
#    define EB_CAPS_OVERLAP_MS 50
#endif
#ifndef EB_CAPS_CHORD_MS
#    define EB_CAPS_CHORD_MS 100
#endif

// Timings of recent Caps + key gestures, for tuning. Holds no key identities.
typedef struct {
    uint16_t gap;     // Caps down -> other key down
    uint16_t overlap; // other key down -> first release of either key
    uint16_t total;   // Caps down -> Caps up
    char     order;   // 'n' other key released first (nested), 'f' Caps released first
    char     outcome; // 'e' Esc, 'c' Ctrl by overlap rule, 'h' Ctrl by QMK hold
} eb_caps_sample_t;

#define EB_CAPS_SAMPLES 8
static eb_caps_sample_t eb_caps_samples[EB_CAPS_SAMPLES];
static uint8_t          eb_caps_sample_next = 0; // total samples written

static struct {
    bool     caps_down;      // Caps is physically down
    bool     has_other;      // another key was pressed while Caps was down
    bool     other_released; // that key was released before Caps
    bool     as_ctrl;        // this Caps tap was converted to Ctrl; undo on release
    bool     sample_open;    // newest sample still needs its outcome
    char     outcome;
    uint16_t caps_time;
    uint16_t other_time;
    uint16_t overlap;
    keypos_t other_key;
} ebc;

// Call from pre_process_record_user(): sees physical events before tap-hold.
static inline void eb_caps_pre_process(uint16_t keycode, keyrecord_t *record) {
    keyevent_t ev = record->event;

    if (keycode == EB_CAPS) {
        if (ev.pressed) {
            ebc.caps_down      = true;
            ebc.caps_time      = ev.time;
            ebc.has_other      = false;
            ebc.other_released = false;
            ebc.overlap        = 0;
            ebc.outcome        = 0;
        } else if (ebc.caps_down) {
            ebc.caps_down = false;
            if (ebc.has_other) {
                if (!ebc.other_released) {
                    ebc.overlap = TIMER_DIFF_16(ev.time, ebc.other_time);
                }
                eb_caps_sample_t *s = &eb_caps_samples[eb_caps_sample_next++ % EB_CAPS_SAMPLES];
                s->gap              = TIMER_DIFF_16(ebc.other_time, ebc.caps_time);
                s->overlap          = ebc.overlap;
                s->total            = TIMER_DIFF_16(ev.time, ebc.caps_time);
                s->order            = ebc.other_released ? 'n' : 'f';
                s->outcome          = ebc.outcome;
                ebc.sample_open     = (ebc.outcome == 0);
            }
        }
        return;
    }

    if (!ebc.caps_down) {
        return;
    }
    if (ev.pressed) {
        if (!ebc.has_other) {
            ebc.has_other  = true;
            ebc.other_time = ev.time;
            ebc.other_key  = ev.key;
        }
    } else if (ebc.has_other && !ebc.other_released && KEYEQ(ev.key, ebc.other_key)) {
        ebc.other_released = true;
        ebc.overlap        = TIMER_DIFF_16(ev.time, ebc.other_time);
    }
}

// Call from get_permissive_hold() for EB_CAPS. Runs when a key pressed during
// the Caps hold is released while Caps is still down; record is the Caps press.
static inline bool eb_caps_permissive_hold(keyrecord_t *record) {
    if (!ebc.has_other || !ebc.other_released) {
        return false;
    }
    return ebc.overlap >= EB_CAPS_OVERLAP_MS || timer_elapsed(record->event.time) >= EB_CAPS_CHORD_MS;
}

// Call from process_record_user() for EB_CAPS. Returns false when it handled the event.
static inline bool eb_caps_process_record(keyrecord_t *record) {
    if (!record->event.pressed) {
        if (ebc.as_ctrl) {
            ebc.as_ctrl = false;
            unregister_mods(MOD_BIT(KC_LEFT_CTRL));
            return false;
        }
        return true;
    }

    if (record->tap.count == 0) {
        // QMK settled Caps as a hold (TAPPING_TERM or permissive hold).
        ebc.outcome = 'h';
        return true;
    }

    // QMK settled Caps as a tap. If this is the first tap and Caps was released
    // while the other key was still down, apply the overlap rule. (A repeat tap,
    // tap.count > 1, is a fresh press with no other key yet, so it stays Esc.)
    bool chord = record->tap.count == 1 && !ebc.caps_down && ebc.has_other && !ebc.other_released && ebc.overlap >= EB_CAPS_OVERLAP_MS;

    if (ebc.sample_open) {
        ebc.sample_open = false;
        eb_caps_samples[(uint8_t)(eb_caps_sample_next - 1) % EB_CAPS_SAMPLES].outcome = chord ? 'c' : 'e';
    }

    if (chord) {
        ebc.as_ctrl = true;
        register_mods(MOD_BIT(KC_LEFT_CTRL));
        return false;
    }
    return true;
}
