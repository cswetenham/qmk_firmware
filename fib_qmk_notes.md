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
action_tapping_process() tries to call process_tapping(). If this returns false, it instead enqueues the record in waiting_buffer_enq. Then regardless it goes through the waiting buffer and calls process_tapping() on each? Any successful calls to process_tapping cause that element to be dequeued, and the first unsuccessful call stops the loop.
TODO understand the waiting buffer better once I know what process_tapping does.

process_tapping() is a nightmare of nested if()s and #ifdefs which I guess is where most of the logic lies.


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
