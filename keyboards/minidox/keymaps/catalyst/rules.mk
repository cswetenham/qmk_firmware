EXTRAKEY_ENABLE = yes      # Audio control and System control(+450)
COMMAND_ENABLE = no        # Commands for debug and configuration
NKRO_ENABLE = yes          # Nkey Rollover - if this doesn't work, see here: https://github.com/tmk/tmk_keyboard/wiki/FAQ#nkro-doesnt-work
# UNICODE_ENABLE = yes     # Unicode - TODO?
# USE_I2C = no             # I guess, whatever, default works?

ifndef QUANTUM_DIR
	include ../../../../Makefile
endif
