#
# The trustzone blinky application requires two separate binaries.
#
# This makefile build these as two Riot builds.
#

all: secure nonsecure

secure:
	make -C secure
	
nonsecure:
	make -C nonsecure

clean:
	make -C secure clean
	make -C nonsecure clean
	
.PHONY: secure nonsecure clean
