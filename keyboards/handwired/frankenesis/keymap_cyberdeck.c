#include "keymap_common.h"
#include "action_layer.h"

// Layer names, for readability
#define _BASE 0
#define _L_FN 1
#define _R_FN 2
#define _L_RAISE 3
#define _R_RAISE 4
#define _L_LOWER 5
#define _R_LOWER 6

// Fillers to make layering more clear
#define ______ KC_TRNS
#define XXXXXX KC_NO

#define LG(KEY) MT(MOD_LGUI, KC_ ## KEY)
#define RG(KEY) MT(MOD_RGUI, KC_ ## KEY)
#define LC(KEY) MT(MOD_LCTL, KC_ ## KEY)
#define RC(KEY) MT(MOD_RCTL, KC_ ## KEY)
#define LA(KEY) MT(MOD_LALT, KC_ ## KEY)
#define RA(KEY) MT(MOD_RALT, KC_ ## KEY)
#define LS(KEY) MT(MOD_LSFT, KC_ ## KEY)
#define RS(KEY) MT(MOD_RSFT, KC_ ## KEY)

#define LAYOUT( \
  k01, k02, k03, k04, k05,    k45, k44, k43, k42, k41, \
  k11, k12, k13, k14, k15,    k55, k54, k53, k52, k51, \
  k21, k22, k23, k24, k25,    k65, k64, k63, k62, k61, \
            k33, k34, k35,    k75, k74, k73            \
  ) \
  { \
        { KC_NO, k05,   k15,   k25,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k04,   k14,   k24,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k03,   k13,   k23,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k02,   k12,   k22,   KC_NO, k35,   KC_NO, KC_NO }, \
        { KC_NO, k01,   k11,   k21,   KC_NO, k34,   KC_NO, KC_NO }, \
        { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, k75,   KC_NO }, \
        { KC_NO, k41,   k51,   k61,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k42,   k52,   k62,   KC_NO, KC_NO, k74,   KC_NO }, \
        { KC_NO, k43,   k53,   k63,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k44,   k54,   k64,   KC_NO, KC_NO, KC_NO, KC_NO }, \
        { KC_NO, k45,   k55,   k65,   KC_NO, KC_NO, KC_NO, KC_NO } \
  }

//!!!!!!!!!!!!!!! THIS IS THE OLD FILE COPIED FROM TMK, SEE keymaps/default/keymap.c FOR THE ACTUAL IMPLEMENTATION!!!!!!!!!!!!

// layer3 on iff layer1 and layer2 are on. Given how layers are ordered,
// higher layers have priority, so layer3 should be > layer1 and layer2.
uint32_t update_tri_layer_state(uint32_t state, uint8_t layer1, uint8_t layer2, uint8_t layer3) {
  uint32_t mask12 = (1UL << layer1) | (1ULL << layer2);
  uint32_t mask3 = 1UL << layer3;
  return (state & mask12) == mask12 ? (state | mask3) : (state & ~mask3);
}

void hook_layer_change(uint32_t layer_state) {
  uint32_t old_layer_state = layer_state;
  layer_state = update_tri_layer_state(layer_state, _L_FN, _L_RAISE, _L_LOWER);
  layer_state = update_tri_layer_state(layer_state, _R_FN, _R_RAISE, _R_LOWER);
  // Prevent infinite recursion
  if (layer_state != old_layer_state) {
    // Tedious...maybe expand update_tri_layer and refactor?
    // layer_state_set(layer_state);
    layer_xor(layer_state ^ old_layer_state);
  }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base - Colemak Mod-DH
 *
 * ,----------------------------------.           ,----------------------------------.
 * |   Q  |   W  |   F  |   P  |   B  |           |   J  |   L  |   U  |   Y  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   A  |   R  |   S  |   T  |   G  |           |   M  |   N  |   E  |   I  |   O  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   Z  |   X  |   C  |   D  |   V  |           |   K  |   H  |   ,  |   .  |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      | Bksp |      |    |      | Space|      |
 *                  | Lower| Raise| Tab  |    | Enter| Raise| Lower|
 *                  `-------------| Fn   |    |  Fn  |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */


/* Base - Qwerty
 *
 * ,----------------------------------.           ,----------------------------------.
 * |   Q  |   W  |   E  |   R  |   T  |           |   Y  |   U  |   I  |   O  |   P  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   A  |   S  |   D  |   F  |   G  |           |   H  |   J  |   K  |   L  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   Z  |   X  |   C  |   V  |   B  |           |   N  |   M  |   ,  |   .  |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      | Bksp |      |    |      | Space|      |
 *                  | Lower| Raise| Tab  |    | Enter| Raise| Lower|
 *                  `-------------| Fn   |    |  Fn  |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_BASE] = LAYOUT( \
  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,      KC_Y, KC_U,  KC_I,    KC_O,   KC_P, \
  LG(A), LA(S), LC(D), LS(F), KC_G,      KC_H, RS(J), RC(K),   RA(L),  KC_RGUI, \
  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,      KC_N, KC_M,  KC_COMM, KC_DOT, XXXXXXX, \
  MO(_L_LOWER), LT(_L_RAISE,KC_BSPC), LT(_L_FN,KC_TAB), LT(_R_FN,KC_ENT), LT(_R_RAISE,KC_SPC), MO(_R_LOWER) \
),

/* Raise Left
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      |   7  |   8  |   9  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |   4  |   5  |   6  |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |   1  |   2  |   3  |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |      |      |    |      |   0  |      |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_L_RAISE] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_7, KC_8, KC_9, XXXXXXX, \
  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_4, KC_5, KC_6, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_1, KC_2, KC_3, XXXXXXX, \
                    _______, _______, _______,      _______, KC_0, _______ \
),

/* Raise Right
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |   `  |   -  |   =  |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |   [  |   ]  |   \  |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |   ;  |   '  |   /  |      |           |      |      |      |      |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |  Esc |      |    |      |      |      |
 *                  `-------------| Caps |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_R_RAISE] = LAYOUT( \
  XXXXXXX, KC_GRV,  KC_MINS, KC_EQL,  XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, XXXXXXX,      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
  XXXXXXX, KC_SCLN, KC_QUOT, KC_SLSH, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                    _______, KC_ESC,  KC_CAPS,      _______, _______, _______ \
),

/* Fn Left
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      | Home | PgDn | PgUp | End  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |   <  |   v  |   ^  |   >  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |  M<  |  Mv  |  M^  |  M>  |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |      |      |    |      |      |      |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
// NOTE: mouse buttons aren't MT().
// NOTE: need to split, otherwise can't hold e.g. up and left at the same time.
// Alternatively could split layers into left and right layers, as in ergodox version.
[_L_FN] =  LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, \
                    _______, _______, _______,      _______, _______, _______  \
),


/* Fn Right
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      | Back | Play | Next |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |LClick|MClick|RClick|      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * | Reset| Vol- | Mute | Vol+ |      |           |      |      |      |      |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |      |      |    |      |      |      |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
// NOTE: mouse buttons aren't MT().
// NOTE: need to split, otherwise can't hold e.g. up and left at the same time.
// Alternatively could split layers into left and right layers, as in ergodox version.
[_R_FN] =  LAYOUT( \
  XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, KC_BTN1, KC_BTN3, KC_BTN2, XXXXXXX,      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
  RESET,   KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                    _______, _______, _______,      _______, _______, _______  \
),


/* Lower Left
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      |  F7  |  F8  |  F9  | F10  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |  F4  |  F5  |  F6  | F11  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |  F1  |  F2  |  F3  | F12  |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |      |      |    |      |      |      |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
// NOTE: didn't implement the mods on the right
[_L_LOWER] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_F7, KC_F8, KC_F9, KC_F10, \
  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_F4, KC_F5, KC_F6, KC_F11, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F12, \
                    _______, _______, _______,      _______, _______, _______ \
),

/* Lower Right
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |      |      |      |      |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      |      |      |    |      |      |      |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
// NOTE: didn't implement the mods on the right
[_R_LOWER] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                    _______, _______, _______,      _______, _______, _______ \
),

};

const action_t PROGMEM fn_actions[] = {};
