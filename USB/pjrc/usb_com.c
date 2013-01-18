/* Copyright (C) 2011 by Jacob Alexander
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// ----- Includes -----

// Compiler Includes
#include <util/delay.h>

// AVR Includes

// Project Includes
#include "usb_keyboard_debug.h"

// Local Includes
#include "usb_com.h"



// ----- Variables -----

// which modifier keys are currently pressed
// 1=left ctrl,    2=left shift,   4=left alt,    8=left gui
// 16=right ctrl, 32=right shift, 64=right alt, 128=right gui
         uint8_t USBKeys_Modifiers = 0;

// which keys are currently pressed, up to 6 keys may be down at once
         uint8_t USBKeys_Array[USB_MAX_KEY_SEND] = {0,0,0,0,0,0};

// The number of keys sent to the usb in the array
         uint8_t USBKeys_Sent;

// 1=num lock, 2=caps lock, 4=scroll lock, 8=compose, 16=kana
volatile uint8_t USBKeys_LEDs = 0;



// ----- Functions -----

// USB Module Setup
inline void usb_setup(void)
{
	// Initialize the USB, and then wait for the host to set configuration.
	// If the Teensy is powered without a PC connected to the USB port,
	// this will wait forever.
	usb_init();
	while ( !usb_configured() ) /* wait */ ;

	// Wait an extra second for the PC's operating system to load drivers
	// and do whatever it does to actually be ready for input
	_delay_ms(1000);
}


// USB Data Send
inline void usb_send(void)
{
		// TODO undo potentially old keys
		for ( uint8_t c = USBKeys_Sent; c < USBKeys_MaxSize; c++ )
			USBKeys_Array[c] = 0;

		// Send keypresses
		usb_keyboard_send();

		// Clear modifiers and keys
		USBKeys_Modifiers = 0;
		USBKeys_Sent      = 0;
}

