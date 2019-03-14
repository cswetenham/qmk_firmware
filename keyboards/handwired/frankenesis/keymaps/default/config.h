#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

#pragma once

#define TAPPING_TERM 100
#undef PERMISSIVE_HOLD
#define IGNORE_MOD_TAP_INTERRUPT
#undef RETRO_TAPPING

// How many taps before triggering the toggle
#define TAPPING_TOGGLE 2
// NOTE undef'ing this means that keys can autorepeat, instead.
#undef TAPPING_FORCE_HOLD
// Allows sending more than one key per scan. By default, only one key event gets sent via process_record() per scan. This has little impact on most typing, but if you're doing a lot of chords, or your scan rate is slow to begin with, you can have some delay in processing key events. Each press and release is a separate event. For a keyboard with 1ms or so scan times, even a very fast typist isn't going to produce the 500 keystrokes a second needed to actually get more than a few ms of delay from this. But if you're doing chording on something with 3-4ms scan times? You probably want this.
#define QMK_KEYS_PER_SCAN 4

#endif