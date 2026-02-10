#include QMK_KEYBOARD_H

// https://beta.docs.qmk.fm/using-qmk/advanced-keycodes/feature_macros
// see process_record_user below for handling of these custom keycodes
enum custom_keycodes {
  MAC_SS = SAFE_RANGE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Layer 0, default layer
__________________________________________________________________________________________________________________________________  ________
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|  ESC   |   F1   |   F2   |   F3   |   F4   |   F5   |   F6   |   F7   |   F8   |   F9   |  F10   |  F11   |  F12   |  DELETE    ||  HOME  |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|  ~`    |   1    |   2    |   3    |   4    |   5    |   6    |   7    |   8    |   9    |   0    |  _ -   | =  +   | BACKSPACE  ||  PGUP  |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|            |        |        |        |        |        |        |        |        |        |        |  [     |   ]    |        ||        |
|     TAB    |   Q    |   W    |   E    |   R    |   T    |   Y    |   U    |   I    |   O    |   P    |  {     |   }    |  |   \ ||  PGDN  |
|____________|________|________|________|________|________|________|________|________|________|________|________|________|________||________|
  |            |        |        |        |        |        |        |        |        |        |   ;    |   '    |            |   |        |
  |  CTRL/ESC  |   A    |   S    |   D    |   F    |   G    |   H    |   J    |   K    |   L    |   :    |   "    |   ENTER    |   |  END   |
  |____________|________|________|________|________|________|________|________|________|________|________|________|____________|___|________|
  |                |        |        |        |        |        |        |        |   ,    |    .   |   /    |            |        |
  |     SHIFT      |   Z    |   X    |   C    |   V    |   B    |   N    |   M    |   <    |    >   |   ?    |   SHIFT    |   UP   |
  |________________|________|________|________|________|________|________|________|________|________|________|____________|________|_________
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |    CTRL    |  LALT  |   FN  | LGUI   |   FN2/SPACE     |      SPACE      | RCTRL  |  RALT  |   FN        |   |  LEFT  |  DOWN  | RIGHT  |
  |____________|________|_______|________|_________________|_________________|________|________|_____________|   |________|________|________|
*/

  [0] = LAYOUT(
//     ESC      F1       F2       F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Del           Home
       KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_DEL,       KC_HOME,

//     `~       1        2        3        4        5        6        7        8        9        0        -_       =+       Bksp          PgUp
       KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC,      KC_PGUP,

//     Tab      Q        W        E        R        T        Y        U        I        O        P        [{       ]}       \|            PgDn
       KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,      KC_PGDN,

//    Esc                A        S        D        F        G        H        J        K        L        ;:       '"        Enter        End
 MT(MOD_LCTL, KC_ESC),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,  KC_ENT,      KC_END,

//            Shift      Z        X        C        V        B        N        M        ,<       .>       /?         Shift        Up
              KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,     KC_UP,

//            Ctrl     Alt      Fn       Super        Fn(lg)          Space       Ctrl       Alt            Fn       Left      Down     Right
              KC_LCTL, KC_LALT, MO(2),   KC_LGUI,  LT(1, KC_SPC),     KC_SPC,     KC_RCTL,   KC_LALT,       MO(1),   KC_LEFT,  KC_DOWN, KC_RGHT
  ),

    /* Layer 1, function layer
__________________________________________________________________________________________________________________________________  ________
|        |        |        |        |        |        |        |        |        |        |        |        |        |            || PLAY/  |
| RESET  |        |        |  F13   |  F14   |  F15   |  F16   |  F17   |  F18   |  F19   |  F20   |  F21   |  F22   | Media <=   || PAUSE  |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|        |        |        |        |        |        |        |        |        |        |  LED   |  LED   |  LED   |            || VOLUME |
|        |        |        |        |        |        |        |        |        |        | TOGGLE |  DOWN  |  UP    | Media =>   ||   UP   |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|            |        |        |        |        |        |        |        |        |        |        |        |        |        || VOLUME |
|PRINT SCREEN|        |        |        |        |        |  HOME  |  PGDN  |  PGUP  |  END   |        |        |        |        ||  DOWN  |
|____________|________|________|________|________|________|________|________|________|________|________|________|________|________||________|
  |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
  |            |        |        |        |        |        |  LEFT  |  DOWN  |   UP   | RIGHT  |        |        | S+A+ENTER  |   |  MUTE  |
  |____________|________|________|________|________|________|________|________|________|________|________|________|____________|___|________|
  |                |        |        |        |        |        |        |        |        |        |        |            |        |
  |                |        |        |        |CtrlShfV|        |        |        |        |        |        |            |  PGUP  |
  |________________|________|________|________|________|________|________|________|________|________|________|____________|________|_________
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |            |        |       |        |                 |                 |        |        |             |   |  HOME  |  PGDN  |  END   |
  |____________|________|_______|________|_________________|_________________|________|________|_____________|   |________|________|________|

* 'QK_BOOT' resets the controller and puts the board into firmware flashing mode. If this key is hit accidentally, just unplug the board
*        and plug it back in.
*/

  [1] = LAYOUT(
//  ESC      F1       F2        F3       F4       F5       F6       F7       F8       F9       F10      F11      F12      Del                   Home
    QK_BOOT, KC_TRNS, KC_TRNS,  KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_MEDIA_PREV_TRACK,  KC_MPLY,

//  `~       1        2         3        4        5        6        7        8        9        0        -_       =+       Bksp                  PgUp
    KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RGB_TOG, RGB_VAD, RGB_VAI, KC_MEDIA_NEXT_TRACK,  KC_VOLU,

//      Tab     Q        W        E        R        T        Y        U        I        O        P        [{       ]}               \|          PgDn
        MAC_SS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_TRNS, KC_TRNS, KC_TRNS,         KC_TRNS,    KC_VOLD,

//        Esc      A        S        D        F        G        H        J        K        L        ;:       '"               Enter             End
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_TRNS,         KC_TRNS,          KC_MUTE,

//          Shift    Z        X        C        V          B        N        M        ,<       .>       /?         Shift               Up
            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, RCS(KC_V), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,            KC_TRNS,

//        Ctrl     Alt      Fn       Super        Fn(lg)            Space         Ctrl     Alt      Fn                        Left     Down     Right
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,          KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,                  KC_TRNS, KC_TRNS, KC_TRNS
  ),

    /* Layer 2 - Windowing / Navigation
____________________________________________________________________________________________________________________________________________
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|            |        |        |        |        |        |        |        |        |        |        |        |        |        ||        |
|            |        |        |        |        |        |        |        |        |        |        |        |        |        ||        |
|____________|________|________|________|________|________|________|________|________|________|________|________|________|________||________|
  |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
  |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
  |____________|________|________|________|________|________|________|________|________|________|________|________|____________|___|________|
  |                |        |        |        |        |        |        |        |        |        |        |            |        |
  |                |        |        |        |        |        |        |        |        |        |        |            |        |
  |________________|________|________|________|________|________|________|________|________|________|________|____________|________|_________
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |____________|________|_______|________|_________________|_________________|________|________|_____________|   |________|________|________|

*/

      [2] = LAYOUT(

//  ESC              F1         F2         F3         F4          F5         F6          F7          F8          F9       F10      F11      F12      Del                           Home
 LGUI(LCTL(KC_Q)),   LCA(KC_U), LCA(KC_I), LCA(KC_D), LCA(KC_F),  LCA(KC_G), HYPR(KC_7), HYPR(KC_8), HYPR(KC_9), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS,

//  `~               1          2          3          4           5          6           7           8           9        0        -_       =+       Bksp                          PgUp
    KC_TRNS,         LCA(KC_J), LCA(KC_K), LCA(KC_E), LCAG(KC_H), LCA(KC_T), HYPR(KC_U), HYPR(KC_I), HYPR(KC_O), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,                      KC_TRNS,

//  Tab              Q             W        E        R        T            Y        U        I        O        P        [{            ]}       \|               PgDn
    LCAG(KC_EQL),    LCAG(KC_F10), KC_TRNS, KC_TRNS, KC_TRNS, LCAG(KC_F2), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, LCAG(KC_F11), KC_TRNS, LCAG(KC_F12),    KC_TRNS,

//    Esc            A        S        D        F        G        H        J        K        L        ;:       '"          Enter                            End
      KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_TRNS, KC_TRNS,    KC_TRNS,                         KC_TRNS,

//    Shift            Z        X        C            V        B        N        M        ,<       .>       /?         Shift            Up
      KC_TRNS,         KC_TRNS, KC_TRNS, LCAG(KC_F1), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,         LCA(KC_ENT),

//    Ctrl           Alt      Fn       Super        Fn(lg)            Space         Ctrl     Alt      Fn             Left           Down           Right
      KC_TRNS,       KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,          KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,       LCA(KC_LEFT),  LCAG(KC_LEFT), LCA(KC_RGHT)
  ),

//     Old Layer 2
//     /* Layer 2, Window Management (OSX - Rectangle Pro)
// ____________________________________________________________________________________________________________________________________________
// |        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
// |Layer0  | LCGA   |  LCGA  |  LCGA  |        |        |        |        |        |        |        |  LCGA  | LCGA   |  LCGA      ||        |
// |________|_(F1)___|__(F2)__|__(F3)__|________|________|________|________|________|________|________|__(F11)_|_(F12)__|___(=)______||________|
// |        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
// |        |        |        |        |        |        |        |HYPER(7)|HYPER(8)|HYPER(9)|        |        | LCGA   |            ||        |
// |________|________|________|________|________|________|________|________|________|________|________|________|__(=)___|____________||________|
// |            |        |        |        |        |        |        |        |        |        |        |        |        |        ||        |
// |OSX ScrnSht | LCA(D) | LCA(F) | LCA(G) |        |        |        |HYPER(U)|HYPER(I)|HYPER(O)|        | LCA(◀) | LCA(▶) |        ||        |
// |____________|________|________|________|________|________|________|________|________|________|________|________|________|________||________|
//   |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
//   |            | LCA(E) | LCGA(H)| LCA(T) |        |        |        |        |        |        |        |        |            |   |        |
//   |____________|________|________|________|________|________|________|________|________|________|________|________|____________|___|________|
//   |                |        |        |        |        |        |        |        |        |        |        |            |        |
//   |                | LCGA(D)|        | LCGA(F)|        |        |        |        |        |        |LCGA(/) |            |LCA(CR) |
//   |________________|________|________|________|________|________|________|________|________|________|________|____________|________|_________
//   |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
//   |            |        |       |        |                 |                 |        |        |             |   |        |LCGA(◀) |        |
//   |____________|________|_______|________|_________________|_________________|________|________|_____________|   |________|________|________|
//
// */
//
//   [2] = LAYOUT(
//                 TO(0), LCTL(LGUI(LALT(KC_F1))), LCTL(LGUI(LALT(KC_F2))), LCTL(LGUI(LALT(KC_F3))), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS,   KC_TRNS, KC_TRNS,   LCTL(LGUI(LALT(KC_F11))), LCTL(LGUI(LALT(KC_F12))), LCTL(LGUI(LALT(KC_EQL))),     KC_TRNS,
//               KC_TRNS,                 KC_TRNS,                 KC_TRNS,                 KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_7), HYPR(KC_8), HYPR(KC_9), KC_TRNS,                    KC_TRNS, LCTL(LGUI(LALT(KC_EQL))),                  KC_TRNS,     KC_TRNS,
//      LSFT(LGUI(KC_5)),            LCTL(LALT(KC_D)),        LCTL(LALT(KC_F)),        LCTL(LALT(KC_G)), KC_TRNS, KC_TRNS, KC_TRNS, HYPR(KC_U), HYPR(KC_I), HYPR(KC_O), KC_TRNS, LCTL(LALT(KC_LEFT)),       LCTL(LALT(KC_RIGHT)), KC_TRNS, KC_TRNS,
//                 KC_TRNS,       LCTL(LALT(KC_E)),                KC_TRNS,       LCTL(LALT(KC_T)), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS,                   KC_TRNS, KC_TRNS,    KC_TRNS,     KC_TRNS,
//                 KC_TRNS, LCTL(LGUI(LALT(KC_D))), LCTL(LGUI(LALT(KC_H))), LCTL(LGUI(LALT(KC_F))), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,          KC_TRNS, LCTL(LGUI(LALT(KC_SLSH))),   KC_TRNS,  LCTL(LALT(KC_ENT)),
//                 KC_TRNS,                KC_TRNS,                KC_TRNS,                KC_TRNS, KC_TRNS, KC_TRNS,               KC_TRNS, KC_TRNS, KC_TRNS,                       KC_TRNS,  LCTL(LGUI(LALT(KC_LEFT))), KC_TRNS
//   ),



    /* Layer 3
____________________________________________________________________________________________________________________________________________
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|        |        |        |        |        |        |        |        |        |        |        |        |        |            ||        |
|________|________|________|________|________|________|________|________|________|________|________|________|________|____________||________|
|            |        |        |        |        |        |        |        |        |        |        |        |        |        ||        |
|            |        |        |        |        |        |        |        |        |        |        |        |        |        ||        |
|____________|________|________|________|________|________|________|________|________|________|________|________|________|________||________|
  |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
  |            |        |        |        |        |        |        |        |        |        |        |        |            |   |        |
  |____________|________|________|________|________|________|________|________|________|________|________|________|____________|___|________|
  |                |        |        |        |        |        |        |        |        |        |        |            |        |
  |                |        |        |        |        |        |        |        |        |        |        |            |        |
  |________________|________|________|________|________|________|________|________|________|________|________|____________|________|_________
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |            |        |       |        |                 |                 |        |        |             |   |        |        |        |
  |____________|________|_______|________|_________________|_________________|________|________|_____________|   |________|________|________|

*/

  [3] = LAYOUT(
    KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,    KC_TRNS,     KC_TRNS,
              KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS,  KC_TRNS,
          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS,          KC_TRNS,      KC_TRNS, KC_TRNS, KC_TRNS,       KC_TRNS,  KC_TRNS, KC_TRNS
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case MAC_SS:
      if (record->event.pressed) {
        SEND_STRING(SS_DOWN(X_LSFT) SS_LGUI("5") SS_UP(X_LSFT));
      }
      break;
  }
  return true;

}
