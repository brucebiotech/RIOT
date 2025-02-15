#
# this builds the SECURE-STATE binary for the modem application
#
# we are not going to use riot
#
APPLICATION = blinky_s
BOARD := fz-nrf9151

#
# should be in a trustzone feature somewhere
#
CFLAGS += -DTRUSTZONE_NONSECURE_STATE=1 -DTRUSTZONE_SECURE_STATE=2

# build for secure state
CFLAGS += -DTRUSTZONE=TRUSTZONE_SECURE_STATE

CFLAGS_OPT ?= -O0

#
# avoid cmsis trustzone error/warning
#
OPTIONAL_CFLAGS_BLACKLIST += -Wcast-align

#
# partition secure flash and ram
#
ROM_OFFSET = 		0x00000000
ROM_LEN := 			0x40000 
RAM_START_ADDR = 	0x20000000
RAM_LEN = 			0x3000

CFLAGS += -DNONSECURE_STATE_VTOR_ADDRESS=$(ROM_LEN) -mcmse

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../../../../..

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

USEMODULE += stdio_uart

# Use a peripheral timer for the delay, if available
FEATURES_OPTIONAL += periph_timer

include $(RIOTBASE)/Makefile.include
