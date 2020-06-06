/* Copyright 2020 Iris Swetenham
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Fillers to make layering more clear
#define _______ KC_TRNS
#define XXXXXXX KC_NO

// From the docs on qmk bluetooth support
// Bluetooth Keycodes
// This is used when multiple keyboard outputs can be selected. Currently this only allows for switching between USB and Bluetooth on keyboards that support both.
// OUT_AUTO Automatically switch between USB and Bluetooth
// OUT_USB USB only
// OUT_BT Bluetooth only

#define LG(KEY) MT(MOD_LGUI, KC_ ## KEY)
#define RG(KEY) MT(MOD_RGUI, KC_ ## KEY)
#define LC(KEY) MT(MOD_LCTL, KC_ ## KEY)
#define RC(KEY) MT(MOD_RCTL, KC_ ## KEY)
#define LA(KEY) MT(MOD_LALT, KC_ ## KEY)
#define RA(KEY) MT(MOD_RALT, KC_ ## KEY)
#define LS(KEY) MT(MOD_LSFT, KC_ ## KEY)
#define RS(KEY) MT(MOD_RSFT, KC_ ## KEY)

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE,
    _L_FN,
    _R_FN,
    _L_RAISE,
    _R_RAISE,
    _L_LOWER,
    _R_LOWER,
    _GAMING
};

// NOTE added tri-layer so we can use just 2 thumb keys

// If FN and RAISE layers are active, then activate LOWER as well on that side.
// Relies on LOWER being a higher layer number than FN and RAISE.
uint32_t layer_state_set_user(uint32_t state) {
  state = update_tri_layer_state(state, _L_FN, _L_RAISE, _L_LOWER);
  state = update_tri_layer_state(state, _R_FN, _R_RAISE, _R_LOWER);
  return state;
}

// TODO gaming layer switched on/off by outermost thumb keys
// TODO another difference between my compiled ergodox layout and the one below is that
// this one has plain modifiers on the off hand of the raise and fn layers, and XXXXXX / KC_NO on the rest,
// but the ergodox has KC_TRANSPARENT / KC_TRNS / ______ for all of them.

// That can be the next thing I test...

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base - Qwerty
 *
 * ,----------------------------------.           ,----------------------------------.
 * |   Q  |   W  |   E  |   R  |   T  |           |   Y  |   U  |   I  |   O  |   P  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   A  |   S  |   D  |   F  |   G  |           |   H  |   J  |   K  |   L  |   ;  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   Z  |   X  |   C  |   V  |   B  |           |   N  |   M  |   ,  |   .  |   /  |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |      | Bksp |      |    |      | Space|      |
 *                  |Gaming| Raise| Tab  |    | Enter| Raise|      |
 *                  `-------------| Fn   |    |  Fn  |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_BASE] = LAYOUT( \
  KC_Q,  KC_W,  KC_E,  KC_R,  KC_T,      KC_Y, KC_U,  KC_I,    KC_O,   KC_P, \
  LG(A), LA(S), LC(D), LS(F), KC_G,      KC_H, RS(J), RC(K),   RA(L),  RG(SCLN), \
  KC_Z,  KC_X,  KC_C,  KC_V,  KC_B,      KC_N, KC_M,  KC_COMM, KC_DOT, KC_SLSH, \
  TO(_GAMING), LT(_L_RAISE,KC_BSPC), LT(_L_FN,KC_TAB), LT(_R_FN,KC_ENT), LT(_R_RAISE,KC_SPC), XXXXXXX \
),
// NOTE added semicolon even though it's redundant, so that i3 shortcut works for motion. Could pick a diff character.

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
 *                  `-------------|      |    |  Del |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_L_RAISE] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_7, KC_8, KC_9, XXXXXXX, \
  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_4, KC_5, KC_6, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_1, KC_2, KC_3, XXXXXXX, \
                    _______, _______, _______,      KC_DEL,  KC_0, _______ \
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
 *                  `-------------|      |    |  Ins |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
// NOTE: mouse buttons aren't MT().
// NOTE: need to split, otherwise can't hold e.g. up and left at the same time.
// Alternatively could split layers into left and right layers, as in ergodox version.
[_L_FN] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END, \
  KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX,      XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, \
                    _______, _______, _______,      KC_INS,  _______, _______  \
),


/* Fn Right
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      | Back | Play | Next | PrScn|           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |LClick|MClick|RClick|      |           |      |      |      |      |      |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * | Reset| Vol- | Mute | Vol+ | Pause|           |      |      |      |      |      |
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
  XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, KC_PSCR,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, KC_BTN1, KC_BTN3, KC_BTN2, XXXXXXX,      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
  RESET,   KC_VOLD, KC_MUTE, KC_VOLU, KC_PAUS,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                    _______, _______, _______,      _______, _______, _______  \
),


/* Lower Left
 *
 * ,----------------------------------.           ,----------------------------------.
 * |      |      |      |      |      |           |      |  F7  |  F8  |  F9  | F10  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |      |      |      |      |      |           |      |  F4  |  F5  |  F6  | F11  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * | Reset|      |      |      |      |           |      |  F1  |  F2  |  F3  | F12  |
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
  RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_F1, KC_F2, KC_F3, KC_F12, \
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
// NOTE: didn't implement the mods on the left
[_R_LOWER] = LAYOUT( \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, KC_RSFT, KC_RCTL, KC_RALT, KC_RGUI, \
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                    _______, _______, _______,      _______, _______, _______ \
),

/* Gaming - Qwerty with left hand rotated so WASD is comfortable. WIP on smaller keyboard...
 * Need shift and ctrl, maybe instead of A and Z.
 * This is for mouse and keyboard games, mostly for ones with fps style controls.
 * So everything needs to fit on left hand...
 * Swap space and backspace. Tab is often a menu/map thing. Alt more rare.
 * Could make left thumbs tab, space, ctrl and have bottom left (z/b) be shift? (a/g) will be more
 * often already bound in the game.
 * TODO need Escape key too! Instead of T?
 *
 * ,----------------------------------.           ,----------------------------------.
 * |  Esc |   Q  |   W  |   E  |   R  |           |   Y  |   U  |   I  |   O  |   P  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |   G  |   A  |   S  |   D  |   F  |           |   H  |   J  |   K  |   L  |   ;  |
 * |------+------+------+------+------|           |------+------+------+------+------|
 * |Shift |   Z  |   X  |   C  |   V  |           |   N  |   M  |   ,  |   .  |   /  |
 * `----------------------------------'           `----------------------------------'
 *                  ,--------------------.    ,------,-------------.
 *                  |  Tab | Space|      |    |      | Bksp |      |
 *                  |      |      | Ctrl |    | Enter|      | Base |
 *                  `-------------|      |    |      |-------------'
 *                                |      |    |      |
 *                                `------'    `------'
 */
[_GAMING] = LAYOUT( \
  KC_ESC,  KC_Q,  KC_W,  KC_E,  KC_R,      KC_Y, KC_U,  KC_I,    KC_O,   KC_P, \
  KC_G,    KC_A,  KC_S,  KC_D,  KC_F,      KC_H, KC_J,  KC_K,    KC_L,   KC_SCLN, \
  KC_LSFT, KC_Z,  KC_X,  KC_C,  KC_V,      KC_N, KC_M,  KC_COMM, KC_DOT, KC_SLSH, \
                  KC_TAB, KC_SPC, KC_LCTL, KC_ENT, KC_BSPC, TO(_BASE) \
),

};
