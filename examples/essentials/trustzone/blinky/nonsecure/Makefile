#
# this builds the nonsecure-state binary for the modem application
#
APPLICATION = modem_ns

# as this stage it has to be a nrf91xx based board
BOARD ?= fz-nrf9151

#
# should be in a trustzone feature somewhere
#
CFLAGS += -DTRUSTZONE_NONSECURE_STATE=1 -DTRUSTZONE_SECURE_STATE=2

# build for nonsecure state
CFLAGS += -DTRUSTZONE=TRUSTZONE_NONSECURE_STATE

CFLAGS_OPT ?= -O0

#
# partition non-secure flash and ram
#
ROM_OFFSET = 		0x00040000
ROM_LEN := 			0x000C0000 
RAM_START_ADDR = 	0x20003000
RAM_LEN = 			0x0003d000

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
