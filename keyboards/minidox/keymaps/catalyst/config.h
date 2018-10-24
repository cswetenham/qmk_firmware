
#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

// Already in parent
// #define NO_DEBUG
// #define NO_PRINT

// I don't use these, might as well turn them off for now
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

// NKRO by default requires to be turned on, this forces it on during keyboard
// startup regardless of EEPROM setting. NKRO can still be turned off but will
// be turned on again if the keyboard reboots.
#define FORCE_NKRO

// Force a key release to be evaluated using the current layer stack instead of
// remembering which layer it came from (used for advanced cases)
// TODO need to think about this one in terms of exactly what my press-release
// sequences might be in different combos
// #define STRICT_LAYER_RELEASE

#if 0
// How long before a tap becomes a hold, if set above 500, a key tapped during
// the tapping term will turn it into a hold too
// TODO fine-tune.
// Option to try: define TAPPING_TERM 100, undef PERMISSIVE_HOLD, define IGNORE_MOD_TAP_INTERRUPT
#define TAPPING_TERM 200

// Makes tap and hold keys work better for fast typers who don't want tapping term set above 500

// This makes it easier for fast typists to use dual-function keys. Without this, if you let go of a held key inside the tapping term, it won't register.

// Example: (Tapping Term = 200ms)

//     SHFT_T(KC_A) Down
//     KC_X Down
//     KC_X Up
//     SHFT_T(KC_A) Up

// With defaults, if above is typed within tapping term, this will emit ax. With permissive hold, if above is typed within tapping term, this will emit X (so, Shift+X).

// TODO I think I want this - at least, this is how I *think* it should work, I'll try it.
#define PERMISSIVE_HOLD

// Makes it possible to do rolling combos (zx) with keys that convert to other keys on hold
#undef IGNORE_MOD_TAP_INTERRUPT

#else // alternative combination of behaviours, maybe better?
#define TAPPING_TERM 100
#undef PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT
#endif

// TODO ideally, I want to set these options so that mods on left hand only affect right hand presses, and vice-versa.
// Might have to write some custom code for that to work - otherwise would need to create an exponential number of layers...

// Tap anyway, even after TAPPING_TERM, if there was no other key interruption between press and release,

// When you hold a dual function key, and haven't pressed anything when you release the key, normally nothing happens. However, if you enable this, if you release the key without pressing another key, it will send the original key, even if it is outside of the tapping term.

// For instance, if you're using LT(2, KC_SPACE), if you hold the key, don't hit anything else and then release it, normally, nothing happens. But with RETRO_TAPPING defined in your config.h, it will send KC_SPACE.
#undef RETRO_TAPPING

// How many taps before triggering the toggle
#define TAPPING_TOGGLE 2

// Makes it possible to use a dual role key as modifier shortly after having been tapped

// NOTE undef'ing this means that keys can autorepeat, instead.
#undef TAPPING_FORCE_HOLD

// Allows sending more than one key per scan. By default, only one key event gets sent via process_record() per scan. This has little impact on most typing, but if you're doing a lot of chords, or your scan rate is slow to begin with, you can have some delay in processing key events. Each press and release is a separate event. For a keyboard with 1ms or so scan times, even a very fast typist isn't going to produce the 500 keystrokes a second needed to actually get more than a few ms of delay from this. But if you're doing chording on something with 3-4ms scan times? You probably want this.
#define QMK_KEYS_PER_SCAN 4


#endif

