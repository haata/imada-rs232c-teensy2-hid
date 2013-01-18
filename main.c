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

// AVR Includes
#include <avr/io.h>
#include <avr/interrupt.h>

// Project Includes
#include <usb_com.h>

#include <led.h>
#include <print.h>



// ----- Defines -----

// ----- Macros -----
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))



// ----- Variables -----



// ----- Functions -----

// Initial Pin Setup, make sure they are sane
inline void pinSetup(void)
{

	// For each pin, 0=input, 1=output
#if defined(__AVR_AT90USB1286__)
	DDRA = 0x00;
#endif
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x00;
	DDRE = 0x00;
	DDRF = 0x00;


	// Setting pins to either high or pull-up resistor
#if defined(__AVR_AT90USB1286__)
	PORTA = 0x00;
#endif
	PORTB = 0x00;
	PORTC = 0x00;
	PORTD = 0x00;
	PORTE = 0x00;
	PORTF = 0x00;
}

int main(void)
{
	// Setup with 16 MHz clock
	CPU_PRESCALE( 0 );

	// Configuring Pins
	pinSetup();

	// Setup USB Module
	usb_setup();

	// Setup ISR Timer for flagging a kepress send to USB
	// Set to 256 * 1024 (8 bit timer with Clock/1024 prescalar) timer
	TCCR0A = 0x00;
	TCCR0B = 0x03;
	TIMSK0 = (1 << TOIE0);

	// Main Detection Loop
	while ( 1 )
	{
	}
}

