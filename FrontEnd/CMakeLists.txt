###| CMAKE |###
#
# CMakeLists.txt build file imada-rs232c frontend
#
###



###
# Project Description
#

#| Project
project( frontend )

#| Target Name (output name)
set( TARGET imada-rs232c )

#| General Settings
cmake_minimum_required( VERSION 2.8 )



###
# Source Defines
#

set( SRCS
	hid_listen.c
	rawhid.c
)


#| Compiler Flags
add_definitions( "-O2 -Wall -DLINUX" )



###
# Build Targets
#

#| Create the executable
add_executable( ${TARGET} ${SRCS} )

