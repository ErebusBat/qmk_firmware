/* Copyright 2025 @ ErebusBat
 *
 * Reproduces the "quick Caps-position tap loses Escape" bug from
 * keyboards/keychron/k2_he/ansi/keymaps/erebusbat/ESC_HERDR_FIX.md.
 *
 * EB_CAPS = MT(MOD_LCTL, KC_ESC) with PERMISSIVE_HOLD enabled.
 * A fast "caps tap immediately followed by another key" roll must still send
 * Escape (then the other key), never a lone Ctrl chord.
 */

#include "keyboard_report_util.hpp"
#include "keycode.h"
#include "keycodes.h"
#include "test_common.hpp"
#include "action_tapping.h"
#include "test_keymap_key.hpp"

using testing::_;
using testing::InSequence;

// Same per-key policy as the erebusbat keymap: retro tapping only for the caps key.
bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    return keycode == CTL_T(KC_ESC);
}

// Same per-key policy intended by the erebusbat keymap: retain permissive hold
// globally, but never let it turn the caps-position key into Ctrl.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    (void)record;
    return keycode != CTL_T(KC_ESC);
}

class ErebusbatCaps : public TestFixture {};

/* Quick solo tap — baseline: a fast press/release alone must send Escape. */
TEST_F(ErebusbatCaps, quick_solo_caps_tap_sends_escape) {
    TestDriver driver;
    InSequence s;
    auto       caps_key = KeymapKey(0, 1, 0, CTL_T(KC_ESC));

    set_keymap({caps_key});

    EXPECT_NO_REPORT(driver);
    caps_key.press();
    idle_for(30);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);
    caps_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

/* Shortest scan-visible solo tap — tap-hold must not impose a minimum duration. */
TEST_F(ErebusbatCaps, one_millisecond_caps_tap_sends_escape) {
    TestDriver driver;
    InSequence s;
    auto       caps_key = KeymapKey(0, 1, 0, CTL_T(KC_ESC));

    set_keymap({caps_key});

    EXPECT_NO_REPORT(driver);
    caps_key.press();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);
    caps_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

/* The reported failure: quick caps tap rolled into the next key.
 * Desired: Escape is sent (caps resolved as tap), then the rolled key. */
TEST_F(ErebusbatCaps, quick_caps_then_letter_roll_sends_escape) {
    TestDriver driver;
    InSequence s;
    auto       caps_key    = KeymapKey(0, 1, 0, CTL_T(KC_ESC));
    auto       regular_key = KeymapKey(0, 2, 0, KC_J);

    set_keymap({caps_key, regular_key});

    /* Press caps */
    EXPECT_NO_REPORT(driver);
    caps_key.press();
    idle_for(30);
    VERIFY_AND_CLEAR(driver);

    /* Roll: press the next key while caps is still down */
    EXPECT_NO_REPORT(driver);
    regular_key.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    /* Release caps quickly (80ms total hold — well under TAPPING_TERM):
     * must resolve as a tap -> Escape, then the rolled key. */
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_REPORT(driver, (KC_J, KC_ESC));
    EXPECT_REPORT(driver, (KC_J));
    caps_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    regular_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}

/* A very fast roll can fully tap the next letter before caps is released.
 * Caps is still under TAPPING_TERM, so this must remain an Escape tap rather
 * than PERMISSIVE_HOLD silently converting it into a Ctrl chord. */
TEST_F(ErebusbatCaps, letter_tapped_within_quick_caps_tap_sends_escape) {
    TestDriver driver;
    InSequence s;
    auto       caps_key    = KeymapKey(0, 1, 0, CTL_T(KC_ESC));
    auto       regular_key = KeymapKey(0, 2, 0, KC_J);

    set_keymap({caps_key, regular_key});

    EXPECT_NO_REPORT(driver);
    caps_key.press();
    idle_for(30);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    regular_key.press();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    EXPECT_NO_REPORT(driver);
    regular_key.release();
    idle_for(10);
    VERIFY_AND_CLEAR(driver);

    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_REPORT(driver, (KC_J, KC_ESC));
    EXPECT_REPORT(driver, (KC_ESC));
    EXPECT_EMPTY_REPORT(driver);
    caps_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}


/* Deliberate hold: caps held well past TAPPING_TERM with the letter tapped
 * mid-hold must still produce a Ctrl chord (permissive hold is disabled for
 * the caps key, so the hold resolves via the TAPPING_TERM timeout instead). */
TEST_F(ErebusbatCaps, deliberate_caps_hold_chords_with_letter) {
    TestDriver driver;
    InSequence s;
    auto       caps_key    = KeymapKey(0, 1, 0, CTL_T(KC_ESC));
    auto       regular_key = KeymapKey(0, 2, 0, KC_J);

    set_keymap({caps_key, regular_key});

    EXPECT_NO_REPORT(driver);
    caps_key.press();
    idle_for(100);
    VERIFY_AND_CLEAR(driver);

    /* The letter stays buffered because permissive hold is disabled for caps. */
    EXPECT_NO_REPORT(driver);
    regular_key.press();
    idle_for(20);
    regular_key.release();
    idle_for(20);
    VERIFY_AND_CLEAR(driver);

    /* TAPPING_TERM resolves the deliberate hold and replays J as Ctrl+J. */
    EXPECT_REPORT(driver, (KC_LEFT_CTRL));
    EXPECT_REPORT(driver, (KC_LEFT_CTRL, KC_J));
    EXPECT_REPORT(driver, (KC_LEFT_CTRL));
    idle_for(TAPPING_TERM);
    VERIFY_AND_CLEAR(driver);

    EXPECT_EMPTY_REPORT(driver);
    caps_key.release();
    run_one_scan_loop();
    VERIFY_AND_CLEAR(driver);
}