###| CMAKE Source Configurator |###
#
# Written by Jacob Alexander 2013 for the Imada RS-232C to USB converter
#
# Released into the Public Domain
#
###



###
# Project Modules
#

##| Sends the current list of usb key codes through USB HID
set(   USBModule  "pjrc"   )

##| Debugging source to use, each module has it's own set of defines that it sets
set( DebugModule  "full"   )




###
# Path Setup
# 
set(   USBModulePath   "USB/${USBModule}"   )
set( DebugModulePath "Debug/${DebugModule}" )

#| Top-level directory adjustment
set( HEAD_DIR "${PROJECT_SOURCE_DIR}" )




###
# Module Configuration
#

#| Additional options, usually define settings
add_definitions()

#| Include path for each of the modules
add_definitions(
	-I${HEAD_DIR}/${USBModulePath}
	-I${HEAD_DIR}/${DebugModulePath}
)




###
# Module Processing
#

#| Go through lists of sources and append paths
#| Usage:
#|  PathPrepend( OutputListOfSources <Prepend Path> <InputListOfSources> )
macro( PathPrepend Output SourcesPath )
	unset( tmpSource )

	# Loop through items
	foreach( item ${ARGN} )
		# Set the path
		set( tmpSource ${tmpSource} "${SourcesPath}/${item}" )
	endforeach( item )

	# Finalize by writing the new list back over the old one
	set( ${Output} ${tmpSource} )
endmacro( PathPrepend )


#| USB Module
include    (             "${USBModulePath}/setup.cmake"  )
PathPrepend(   USB_SRCS   ${USBModulePath} ${USB_SRCS}   )

#| Debugging Module
include    (           "${DebugModulePath}/setup.cmake"  )
PathPrepend( DEBUG_SRCS ${DebugModulePath} ${DEBUG_SRCS} )


#| Print list of all module sources
message( STATUS "Detected USB Module Source Files:" )
message( "${USB_SRCS}" )
message( STATUS "Detected Debug Module Source Files:" )
message( "${DEBUG_SRCS}" )

