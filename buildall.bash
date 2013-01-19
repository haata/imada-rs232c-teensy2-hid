#!/bin/bash
###| Builder Script |###
# 
# Written by Jacob Alexander 2013 for imada-rs232c-teensy2-hid
# Released into the Public Domain
#
###

## TODO List ##
# - TODO


main() {
	ERROR="\e[5;1;31mERROR\e[0m:"

	# Prepare the build directory
	mkdir -p build/FrontEnd
	cd build

	# First attempt to build the FrontEnd (more likely to succeed :P)
	cd FrontEnd
	cmake ../../FrontEnd
	make
	cd ..

	# Next build the Teensy 2.0 firmware
	cmake ..
	make

	# Symlink FrontEnd executable
	if [ ! -f imada-rs232c ]; then
		ln -s FrontEnd/imada-rs232c .
	fi
}


#| Main Script Entry
main "$@"


exit 0

