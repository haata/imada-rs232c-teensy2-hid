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

#include <util/delay.h>

// Project Includes
#include <usb_com.h>

#include <led.h>
#include <print.h>



// ----- Defines -----

// ----- Macros -----
#define CPU_PRESCALE(n)	(CLKPR = 0x80, CLKPR = (n))

#define ADC_BUFSIZE 32

#define ADC_PRESCALER ( (1 << ADPS2) | (1 << ADPS1) )


// ----- Variables -----

static volatile uint8_t  adc_head, adc_tail;
static volatile uint16_t adc_buffer[ADC_BUFSIZE];



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


	// ------ USART Setup --------

	// Setup USART interface for RS-232C
	// Setup baud rate
	// 16 MHz / ( 16 * baud ) = UBRR
	// Baud <- 2400 bps -> 416.6666667
	// Thus baud setting is 417
	uint16_t baud = 417; // Max setting of 4095
	UBRR1H = (uint8_t)(baud >> 8);
	UBRR1L = (uint8_t)baud;

	// Enable the receiver, and RX Complete Interrupt as well as the transmitter
	UCSR1B = 0x98;

	// Set frame format: 8 data, 1 stop bit, no parity
	// Asynchrounous USART mode
	UCSR1C = 0x06;


	// -------- ADC Setup --------

	uint8_t mux = 0x07; // ADC7 (F7) /w 1x Gain

	// Setup ADC
	ADCSRA = (1 << ADEN)  | ADC_PRESCALER; // Enable ADC, no interrupt
	ADCSRB = 0x80;                         // High speed mode
	ADMUX  = (1 << REFS0) | (1 << REFS1) | (mux & 0x1F);  // Configure the ADC(s) to use, and reference voltage source (VCC)

	// Prepare the buffer
	adc_head = 0;
	adc_tail = 0;

	// Start the ADC in Free Running mode, triggering an interrupt on each sample
	ADCSRA = (1 << ADSC) | (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | ADC_PRESCALER;
}


// ----- Interrupt Functions -----

// USART Receive Buffer Full Interrupt
ISR(USART1_RX_vect)
{
	cli(); // Disable Interrupts

	uint8_t value = 0x00;

	// Read incoming byte value from force gauge
	value = UDR1;

	// TODO

	sei(); // Re-enable Interrupts
}


// ADC Sampling Function
ISR( ADC_vect )
{
	uint8_t pos;
	int16_t val;

	val = ADC;
	pos = adc_head + 1;

	// Handle loop-around case of the circular buffer
	if ( pos >= ADC_BUFSIZE )
		pos = 0;

	// Add to circular buffer, if not full
	if ( pos != adc_tail )
	{
		adc_buffer[pos] = val;
		adc_head = pos;
	}
}


// ----- Functions -----

int main(void)
{
	// Setup with 16 MHz clock
	CPU_PRESCALE( 0 );

	// Configuring Pins
	pinSetup();

	// Setup USB Module
	usb_setup();

	// Main Detection Loop
	while ( 1 )
	{
		// -------- ADC Scanning --------
		uint8_t tail;
		int16_t val;

		tail = adc_tail;

		// Check for new samples in the ADC buffer
		if ( adc_head != adc_tail )
		{
			// Pop one sample from the buffer
			if ( ++tail >= ADC_BUFSIZE )
				tail = 0;
			val = adc_buffer[tail];
			adc_tail = tail;

			// Print ADC value
			char tmpStr[6];
			int16ToStr( val, tmpStr );
			dPrintStrs( tmpStr, "," );
		}

		_delay_ms( 10 );
	}
}

