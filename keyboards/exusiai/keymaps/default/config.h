/* Copyright 2018 REPLACE_WITH_YOUR_NAME
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

#pragma once

// place overrides here

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

#define TAPPING_TERM 100
#undef PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT

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

