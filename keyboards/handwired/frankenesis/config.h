/*
Copyright 2018 REPLACE_WITH_YOUR_NAME

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x0000
#define DEVICE_VER      0x0001
#define MANUFACTURER    IrisWorks
#define PRODUCT         Frankenesis
#define DESCRIPTION     QMK keyboard firmware for hack of Kinesis Advantage

/* key matrix size */
#define MATRIX_ROWS 12
#define MATRIX_COLS 8

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
*/
#define MATRIX_ROW_PINS { F7, F6, F5, F4, F3, F2, F1, F0, B0, B1, B2, B3 }
#define MATRIX_COL_PINS { C0, C1, C2, C3, C4, C5, C6, C7 }
#define UNUSED_PINS

/* COL2ROW, ROW2COL, or CUSTOM_MATRIX */
#define DIODE_DIRECTION COL2ROW

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
// NOTE it looks like at some point the code changed from 'DEBOUNCE' to 'DEBOUNCING_DELAY' with a better impl in
// quantum/matrix.c, but older keyboards with their own matrix.c still use 'DEBOUNCE' instead.
// NOTE ergodox ez defines this as 15 intead, could try a that although at best it would have fixed the dodgy blues,
// I think the coppers are fine.
#define DEBOUNCING_DELAY 5

/* define if matrix has ghost (lacks anti-ghosting diodes) */
//#define MATRIX_HAS_GHOST

#define TAPPING_TERM    200

// this makes it possible to do rolling combos (zx) with keys that convert to other keys on hold
// (z becomes ctrl when you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x.
// That's bad.)
#define IGNORE_MOD_TAP_INTERRUPT

#if 0 //!!!!!!!
// These are what the minidox settings *used to be* but don't feel right. I don't know what the settings are currently that
// are flashed into it. Neither T440s nor work laptop had consistent-looking changes.

#undef PERMISSIVE_HOLD
#undef RETRO_TAPPING

// How many taps before triggering the toggle
#define TAPPING_TOGGLE 2
// NOTE undef'ing this means that keys can autorepeat, instead.
#undef TAPPING_FORCE_HOLD
// Allows sending more than one key per scan. By default, only one key event gets sent via process_record() per scan. This has little impact on most typing, but if you're doing a lot of chords, or your scan rate is slow to begin with, you can have some delay in processing key events. Each press and release is a separate event. For a keyboard with 1ms or so scan times, even a very fast typist isn't going to produce the 500 keystrokes a second needed to actually get more than a few ms of delay from this. But if you're doing chording on something with 3-4ms scan times? You probably want this.
#define QMK_KEYS_PER_SCAN 4

/*
 * Force NKRO
 *
 * Force NKRO (nKey Rollover) to be enabled by default, regardless of the saved
 * state in the bootmagic EEPROM settings. (Note that NKRO must be enabled in the
 * makefile for this to work.)
 *
 * If forced on, NKRO can be disabled via magic key (default = LShift+RShift+N)
 * until the next keyboard reset.
 *
 * NKRO may prevent your keystrokes from being detected in the BIOS, but it is
 * fully operational during normal computer usage.
 *
 * For a less heavy-handed approach, enable NKRO via magic key (LShift+RShift+N)
 * or via bootmagic (hold SPACE+N while plugging in the keyboard). Once set by
 * bootmagic, NKRO mode will always be enabled until it is toggled again during a
 * power-up.
 *
 */
#define FORCE_NKRO

#else //!!!!!!!
// These are the settings that are the default in the ergodox ez firmware. Since the layout feels fine on the ez with the
// layout compiled online, this is what I'm going to try next.
// Edit: wow, with these settings it's perfect, I have no idea which part did the trick...

// How many taps before triggering the toggle
#define TAPPING_TOGGLE  1

#define DISABLE_SPACE_CADET_ROLLOVER

#define PREVENT_STUCK_MODIFIERS

#endif //!!!!!!!

/* number of backlight levels */

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/*
 * Magic Key Options
 *
 * Magic keys are hotkey commands that allow control over firmware functions of
 * the keyboard. They are best used in combination with the HID Listen program,
 * found here: https://www.pjrc.com/teensy/hid_listen.html
 *
 * The options below allow the magic key functionality to be changed. This is
 * useful if your keyboard/keypad is missing keys and you want magic key support.
 *
 */

/* key combination for magic key command */
#define IS_COMMAND() ( \
    keyboard_report->mods == (MOD_BIT(KC_LSHIFT) | MOD_BIT(KC_RSHIFT)) \
)

#if 0 // Maybe an electrical issue but goes a bit weird when I close the lid...
/* PS/2 mouse */
#ifdef PS2_USE_BUSYWAIT
#   define PS2_CLOCK_PORT  PORTD
#   define PS2_CLOCK_PIN   PIND
#   define PS2_CLOCK_DDR   DDRD
#   define PS2_CLOCK_BIT   1
#   define PS2_DATA_PORT   PORTD
#   define PS2_DATA_PIN    PIND
#   define PS2_DATA_DDR    DDRD
#   define PS2_DATA_BIT    2
#endif


/* PS/2 mouse interrupt version */
#ifdef PS2_USE_INT
/* uses INT1 for clock line(ATMega32U4) */
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   1
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    2

#define PS2_INT_INIT()  do {    \
    EICRA |= ((1<<ISC11) |      \
              (0<<ISC10));      \
} while (0)
#define PS2_INT_ON()  do {      \
    EIMSK |= (1<<INT1);         \
} while (0)
#define PS2_INT_OFF() do {      \
    EIMSK &= ~(1<<INT1);        \
} while (0)
#define PS2_INT_VECT    INT1_vect
#endif


/* PS/2 mouse USART version */
#ifdef PS2_USE_USART
#if defined(__AVR_ATmega16U4__) || defined(__AVR_ATmega32U4__) || defined(__AVR_AT90USB1286__)
/* XCK for clock line and RXD for data line */
#define PS2_CLOCK_PORT  PORTD
#define PS2_CLOCK_PIN   PIND
#define PS2_CLOCK_DDR   DDRD
#define PS2_CLOCK_BIT   5
#define PS2_DATA_PORT   PORTD
#define PS2_DATA_PIN    PIND
#define PS2_DATA_DDR    DDRD
#define PS2_DATA_BIT    2

/* synchronous, odd parity, 1-bit stop, 8-bit data, sample at falling edge */
/* set DDR of CLOCK as input to be slave */
#define PS2_USART_INIT() do {   \
    PS2_CLOCK_DDR &= ~(1<<PS2_CLOCK_BIT);   \
    PS2_DATA_DDR &= ~(1<<PS2_DATA_BIT);     \
    UCSR1C = ((1 << UMSEL10) |  \
              (3 << UPM10)   |  \
              (0 << USBS1)   |  \
              (3 << UCSZ10)  |  \
              (0 << UCPOL1));   \
    UCSR1A = 0;                 \
    UBRR1H = 0;                 \
    UBRR1L = 0;                 \
} while (0)
#define PS2_USART_RX_INT_ON() do {  \
    UCSR1B = ((1 << RXCIE1) |       \
              (1 << RXEN1));        \
} while (0)
#define PS2_USART_RX_POLL_ON() do { \
    UCSR1B = (1 << RXEN1);          \
} while (0)
#define PS2_USART_OFF() do {    \
    UCSR1C = 0;                 \
    UCSR1B &= ~((1 << RXEN1) |  \
                (1 << TXEN1));  \
} while (0)
#define PS2_USART_RX_READY      (UCSR1A & (1<<RXC1))
#define PS2_USART_RX_DATA       UDR1
#define PS2_USART_ERROR         (UCSR1A & ((1<<FE1) | (1<<DOR1) | (1<<UPE1)))
#define PS2_USART_RX_VECT       USART1_RX_vect
#endif
#endif

// Scrolling with mouse3 held
// TODO it's possible this only works when the 'true' mouse button 3 is pressed (the wires soldered to the trackpoint module), and ignored if it's the BTN3!
// Could fix this by creating a custom action for the key, and setting the mouse report manually as described in docs/feature_pointing_device.md
#define PS2_MOUSE_ENABLE_SCROLLING
// #define PS2_MOUSE_SCROLL_BTN_MASK (1<<PS2_MOUSE_BUTTON_MIDDLE)
#define PS2_MOUSE_SCROLL_BTN_MASK (1<<2)
#endif

/*
 * Feature disable options
 *  These options are also useful to firmware size reduction.
 */

/* disable debug print */
#ifndef NO_DEBUG
  #define NO_DEBUG
#endif

/* disable print */
#ifndef NO_PRINT
  #define NO_PRINT
#endif

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
#define NO_ACTION_ONESHOT
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION

/*
 * MIDI options
 */

/* Prevent use of disabled MIDI features in the keymap */
//#define MIDI_ENABLE_STRICT 1

/* enable basic MIDI features:
   - MIDI notes can be sent when in Music mode is on
*/
//#define MIDI_BASIC

/* enable advanced MIDI features:
   - MIDI notes can be added to the keymap
   - Octave shift and transpose
   - Virtual sustain, portamento, and modulation wheel
   - etc.
*/
//#define MIDI_ADVANCED

/* override number of MIDI tone keycodes (each octave adds 12 keycodes and allocates 12 bytes) */
//#define MIDI_TONE_KEYCODE_OCTAVES 1

/*
 * HD44780 LCD Display Configuration
 */
/*
#define LCD_LINES           2     //< number of visible lines of the display
#define LCD_DISP_LENGTH    16     //< visibles characters per line of the display

#define LCD_IO_MODE      1            //< 0: memory mapped mode, 1: IO port mode

#if LCD_IO_MODE
#define LCD_PORT         PORTB        //< port for the LCD lines
#define LCD_DATA0_PORT   LCD_PORT     //< port for 4bit data bit 0
#define LCD_DATA1_PORT   LCD_PORT     //< port for 4bit data bit 1
#define LCD_DATA2_PORT   LCD_PORT     //< port for 4bit data bit 2
#define LCD_DATA3_PORT   LCD_PORT     //< port for 4bit data bit 3
#define LCD_DATA0_PIN    4            //< pin for 4bit data bit 0
#define LCD_DATA1_PIN    5            //< pin for 4bit data bit 1
#define LCD_DATA2_PIN    6            //< pin for 4bit data bit 2
#define LCD_DATA3_PIN    7            //< pin for 4bit data bit 3
#define LCD_RS_PORT      LCD_PORT     //< port for RS line
#define LCD_RS_PIN       3            //< pin  for RS line
#define LCD_RW_PORT      LCD_PORT     //< port for RW line
#define LCD_RW_PIN       2            //< pin  for RW line
#define LCD_E_PORT       LCD_PORT     //< port for Enable line
#define LCD_E_PIN        1            //< pin  for Enable line
#endif
*/

