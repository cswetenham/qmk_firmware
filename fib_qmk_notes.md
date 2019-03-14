# Trace of a mod-tap key vs a normal key

In my keymap, I have these keys:

LG(A) on qwerty A position
-- #define LG(KEY) MT(MOD_LGUI, KC_ ## KEY) --> (simplifying)
MT(MOD_LGUI, KC_A)
-- #define MT(mod, kc) (QK_MOD_TAP | (((mod) & 0x1F) << 8) | ((kc) & 0xFF)) --> (simplifying)
QK_MOD_TAP | (MOD_LGUI << 8) | KC_A


And plain ol'
KC_Z on qwerty Z position

I think KC_A and KC_Z, defined in keycode.h, correspond directly to HID keycodes.

These are entries in layer 0 at a certain row and column (should be rows 1 and 2 column 0 I think). The array I define is accessed through uint16_t keymap_key_to_keycode(layer, keypos) in keymap_common.c.
I note that keymap_key_to_keycode is defined with the weak attribute and so a keyboard layout could redefine it, I can't really think of any useful reason to right now. If DYNAMIC_KEYMAP_ENABLE feature is turned on, then that will override keymap_key_to_keycode.
keymap_key_to_keycode is called by process_record_quantum in quantum.c, which is in turn called by process_record in action.c. The keycode is passed to process_record_kb and thence to process_record_user, if those are defined. If they aren't or if they don't handle the keycode themselves, process_record_quantum checks if it's one of the quantum special keycodes: shifted symbols, rbg actions, etc. If not it returns to process_record.

NOTE: I'm still trying to figure out here how the QK_MOD_TAP and MOD_LGUI come into the picture. And what happened to the keycode?

Okay, the keymap is *also* accessed in action_for_key(layer, keypos). This in turn is called in store_or_get_action(pressed, keypos), layer_switch_get_layer(keypos), and layer_switch_get_action(keypos).
layer_switch_get_layer() is the call that looks at all active layers, starting at the highest numbered, and returns the number of the first layer it finds which doesn't have the key defined as transparent.

A bunch of code around here depends on PREVENT_STUCK_MODIFIERS. I don't currently have it defined, according to the docs the idea is that you store the layer a key press came from, and use that layer when the key is released no matter which layers have become active or inactive in the meantime. That sounds like a good idea in general tbh, but it involves yet more machinery I don't yet understand. store_or_get_action() is what wraps the caching of layers.

action_for_key finally *interprets* the keycode. 
case QK_MOD_TAP ... QK_MOD_TAP_MAX:
    mod = mod_config((keycode >> 0x8) & 0x1F); // MOD_LGUI in this case
    action.code = ACTION_MODS_TAP_KEY(mod, keycode & 0xFF); // #define ACTION_MODS_TAP_KEY(mods, key)  ACTION(ACT_MODS_TAP, ((mods)&0x1f)<<8 | (key)) ----> ACTION(ACT_MODS_TAP, (MOD_LGUI << 8) | KC_A)
    // TODO #define ACTION(kind, param)             ((kind)<<12 | (param))
    break;

NOTE mod_config and keycode_config handle weird options like 'swap left and right' and 'swap alt and gui', but by default just do nothing.

bool is_tap_key(keypos_t key) in action.c let code handle tap keys specially 'early' in the sequence. it calls layer_switch_get_action() and then switches on the action.kind field.

I think:
- I should figure out specifically what IGNORE_MOD_TAP_INTERRUPT does
- process_tapping() outputs a record that includes a tap count, and this is what is used by process_action to implement e.g. layer/mod -oneshot, -toggle, -tap, behaviours

Here is how we process a MT key record in process_action(). Unfortunately this is in a nested switch statement I think and there is post-processing afterwards for a few things around layer changes, so can't just return early.

But basic logic is: if pressed, tap count > 0 means send keycode, tap count == 0 means send mod. if released, send the corresponding release code.

This implies that once we call process_action (Does process_record always call it? When does it return false?), we'll need to send the corresponding release event for that key with the
*same* tap_record (except positive tap counts can change, but not go to 0).

If interruption is enabled, then it only applies on the press and not the release; if the press was interrupted then even with a tap count > 0, we cancel the tap in the record and send the modifier instead.

if (event.pressed) {
    if (tap_count > 0) {
#ifndef IGNORE_MOD_TAP_INTERRUPT
        if (record->tap.interrupted) {
            dprint("mods_tap: tap: cancel: add_mods\n");
            // ad hoc: set 0 to cancel tap
            record->tap.count = 0;
            register_mods(mods);
        } else
#endif
        {
            dprint("MODS_TAP: Tap: register_code\n");
            register_code(action.key.code);
        }
    } else {
        dprint("MODS_TAP: No tap: add_mods\n");
        register_mods(mods);
    }
} else {
    if (tap_count > 0) {
        dprint("MODS_TAP: Tap: unregister_code\n");
        unregister_code(action.key.code);
    } else {
        dprint("MODS_TAP: No tap: add_mods\n");
        unregister_mods(mods);
    }
}

When do we assign tapping_key?
Aside from error clearing if we fill the waiting buffer:
- If there is no tapping key, and we press a key, and it's a tap key, when we assign the press event as the tapping key.
- If there is a tapping key, and the tapping key is released (when does this happen?), and the current event (could be a tick?) is outside the tapping term, clean the tapping key.
ALSO we return false, because this record needs to be re-processed.
- If there is a tapping key and the tapping key is released, and within the tapping term, the tapping key is pressed again, if TAPPING_FORCE_HOLD is defined, we set the tapping key to the new (pressed, latest timer) key event.
- If there is a tapping key and the tapping key is released, and within the tapping term, the tapping key is pressed again, if TAPPING_FORCE_HOLD is not defined, and the tapping key is not interrupted, and the tapping key tap count > 0, then we set the tapping key to the new (pressed, latest timer) key event, AND set the record of the current event to the current tap key event, after incrementing the counter by one. I don't know whether the buffer will contain old records for the same key, with lower tap counts?
- If there is a tapping key and the tapping key is released, and within the tapping term, a different tap key is pressed, then set the tapping key to the new key, and scan the waiting buffer for taps?? We're looking for: an old event, where the NEW tapping key is RELEASED, WITHIN the tapping term of the new tapping key (retroactively), starting from the oldest. For the first one we find within the tapping term, we set the tap count of the current tap key and the old record both to 1, and process_record() the new tap key? If there's no such event in the buffer, we won't call process_record for this event yet. We DO NOT set the .interrupted flag.
- If there is a tapping key and the tapping key is released, and within the tapping term, a different non-tap key is pressed, then set the .interrupted flag on the current tap key, but then process_record() the new press as normal.

Here's what I think: if a modtap key is held, and we are within the tapping term, then it counts as a press; otherwise it counts as a hold of the mod.
The naive implementation would be:
- A modtap key is pressed, from a blank initial state. We can't do anything with this yet, so store it away somehow.
- If the modtap key is released, there must have been a corresponding press. We check when it was;
  - If it was within the tapping term, send the press AND release.
  - If it wasn't, clear the stored state.
- If another key is pressed, and the modtap key is currently pressed:
  - If we're within the tapping term, clear the stored state. 
  - If we're not within the tapping term, send the modifier press, then the new key (and maybe the modifier release? or send that later).

Does that help us make sense of the existing implementation?

// M-od, T-ap - 256 keycode max
#define MT(mod, kc) (QK_MOD_TAP | (((mod) & 0x1F) << 8) | ((kc) & 0xFF))

# Split-hand ModTap

Hooking into process_record_user lets you perform certain actions: change layer, play a sound, not sure 'safe calls' are well defined. Of particular interest: add_mods, clear_mods etc; and send_keyboard_report().

*if* I have enough info in process_record_user to tell what keys are being pressed, I could override further processing and send the right keycodes.

Otherwise, ideally I would check, 'is this part of the right hand? then call a method to interrupt the tap interval for anything else in the right hand.'...Ah, no.

When I press, e.g., A then S, I don't know yet if I'm typing 'asteroid' or 'gui+alt'.
If I then press T, I want (at some point, either immediately or when some subset has been released, not sure) to send 'A' 'S' 'T' keypresses (with releases in between or at some point) in the correct order.
If I instead press J, I want to send 'GUI' 'ALT' 'J' (and then releases) in the correct order.

The existing code for handling mod-tap and layer-tap must deal with something similar. I may need to modify the
handling of mod-tap to achieve what I want. The code is pretty fiddly to trace through but patient study should be enough; at this level the code is all just arrays, bitwise ops, function calls.

## action_tapping.c

action_exec() in action.c calls action_tapping_process() after doing some oneshot processing.
action_exec() itself is called from keyboard_task() in keyboard.c, after the matrix scan, once per changed matrix entry, and also once with argument (TICK) at the end. (So things like timeouts can be handled?)
If NO_ACTION_TAPPING is not defined, then ALL presses and releases will be processes through action_tapping_process().
action_tapping_process() tries to call process_tapping(). If this returns false, it instead enqueues the record in waiting_buffer_enq. 
TODO understand the waiting buffer better once I know what process_tapping does.

process_tapping() is a nightmare of nested if()s and #ifdefs which I guess is where most of the logic lies.

Okay, after another look, process_tapping() is tractable. For the most part it's a tree of ifs() with a return; at each leaf, so it can be rearranged.
Let's take another look at the buffer. It's a ring buffer with head and tail indices. If process_tapping() returns false for the new key event in action_tapping_process(), then we enqueue the new event at the HEAD of the queue, which grows in the positive direction. The keyrecord_t is copied into the queue.

Then regardless of whether the current event was enqueued, it goes through the waiting buffer starting from the tail and calls process_tapping() on each. This implies that the same record will have process_record called on it multiple times.
Any successful calls to process_tapping cause that element to be dequeued, and the first unsuccessful call stops the loop.

So far, this suggests that process_tapping() must be responsible for sending onward the key presses and releases to the next layer, and it is: it calls process_record() with the corresponding new or buffered record, and sometimes also with a syntesised key release record.

Both process_tapping() and process_record() take a *non-const pointer* to a record, which I don't like very much; when do they potentially modify the record?

When do we return false from process_tapping?

if (IS_TAPPING_RELEASED() && !WITHIN_TAPPING_TERM(event))

Because of mutation of tapping_key I'm not sure what this means yet. In this case, we clear the tapping_key.

if (IS_TAPPING_PRESSED() && WITHIN_TAPPING_TERM(event) && tapping_key.tap.count == 0 && IS_TAPPING_KEY(event.key) && !event.pressed)

That's a lot of conditions. In this case, we copy the tap subrecord of the current tap key into the supplied record?? What is the tap subrecord exactly?

keypos_t: row and column in matrix.
keyevent_t: press or release, keypos, timestamp.
tap_t: interrupted flag, counter.
keyrecord_t: event and tap record.

tap.count:
- Is tested against == 0, > 0, > 1.
- Is set to 0 or 1, or incremented saturating at 15.

process_tapping() MAY modify keyp->tap, and DOES do so under a set of circumstances I don't yet fully understand.
process_tapping() itself does not modify keyp->event. It takes a copy of it at the start, maybe just for convenience since event is never referred to at a point *after* process_record could have mofidied keyp.

Ah, some of the logic around tapping is defined in process_action() and that will *also* modify keyp->tap.

process_record() will also call process_record_quantum() which then calls process_ functions for a billion features that all could potentially modify the record...searching for 'record->' suggests that this isn't used to modify the event anywhere, but some keymaps do some kind of macro implementation that then explicitly sets record->tap.count = 0. I think it's mostly safe to assume that the 'event' part of a keyrecord_t doesn't get modified after creation.


For the most part, process_tapping does not concern itself with the action associated with a key - except in one specific case that has to do with repeating and I don't understand yet.

TODO walk through a few simple key press and release combinations to make sense of the code:

vZ, ^Z - z is not a tap key

vZ, ..., ^Z

vA, ^A - a is a modtap(gui) key

vA, ..., ^A




# Other stuff

Combos are a way aiui to define custom combinations of keys e.g. a+d -> something, rather than involving layers.
Chords are....??? Not documented? possibly related to steno and also to midi? Or same name used in many places.

# Build system

A top-level Makefile is invoked with a colon-separated target such as lets_split/rev2:YOUR_KEYMAP_NAME:avrdude.
The top-level makefile disables the usual make target parsing and does its own parsing, by creating a single rule that matches everything.

There are many uses of $$(eval $$(call ...)) that I don't understand but I think what ends up happening is that a recursive call to
make -f build_keyboard.mk
is performed, with lots of extra command line args.

build_keyboard.mk has some logic to do with chibios. If I want to try to port qmk to freertos instead (which is what I think is loaded on the adafruit nrf52s? Or is it just what's linked in with the adafruit nrf52 arduino stuff?) then I would need to look here. Otherwise there's also the chibios for nrf52 port...

As a fallback the UART friend, the teensy++ and the io mux should be usable if I can't get the nrf52 working.

So, to use ChibiOS I need to clone the chibios and chibios-contrib repos into the correct location in the qmk tree...and perhaps I will have to twiddle all the bluetooth stuff myself...

I ran:
git submodule sync --recursive && git submodule update --init --recursive

These checked out the chibios and chibios-contrib repos in qmk_firmware/lib/chibios/

I think FreeRTOS is being used as part of the arduino core linked into sketches, so I should be able to use ChibiOS instead without flashing firmware. On the other hand, this means I might be SOL with the Adafruit bluetooth libraries except as a source code reference, when it comes to implementing bluetooth?

Related note, the adafruit page for the UART friend says that it can be programmed to be any HID or other BLE device using AT codes; but also the example of qmk + bluetooth uses an Ez-Key instead.

comedy option: connect left + right with a 10+ wire ribbon cable...

Looking in the qmk source tree, there is a standalone? protocol for bluefruit and a few files in the lufa protocol dir for adafruit ble. Neither explain exactly what product they refer to - to be investigated (and do a global case insensitive search for 'adafruit' and 'bluefruit').

Okay, the makefiles are more helpful. The explicitly supported products are:
Adafruit EZ-Key
Adafruit Feather 32U4 BLE

the 32U4 BLE sounds like it might be what I should have bought in the first place...

For understanding chibios, the k-type (keyboards/k_type) is a pretty normal keyboard I can look at.

I think the pjrc driver is specifically for avr-based teensys like the teensy 2.0, and chibios is used for arm-based teensys like teensy 3.0.

struct host_driver_t is a struct of functions defining how we actually communicate keypresses (and other things?) to the host.

ChibiOS port just has pin assignments, demo of initing the board and doing some uarts. No example of BLE. Would have to call Nordic SDK stuff directly? Was no at all obvious how this works from looking at the Adafruit code.

Other option: write my own keyboard driver as an Arduino sketch...
