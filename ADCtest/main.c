/*
 * main.c
 *
 *  Created on: Jun 17, 2015
 *      Author: jimparker
 */

#undef __AVR_ATmega16__
#include <avr/io.h>
#include <util/delay.h>
#include "pinDefines.h"
#include "USART.h"
#include "ultraSonic.h"


static inline void initADC0(void) {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}
/*
void calibrateADC0 {
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf
	printString("Set the ADC to its lowest setting and hit <enter>!");
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf
	uint8_t = readByte();

}
*/
int main(void) {
	uint8_t ledValue;
	uint16_t adcValue;
	uint8_t i;

	initUSART();
	initADC0();
	LED_DDR = 0xff;
	DDRD &= ~(1 << PD6);    // set button port to input
	PORTD |= (1 << PD6);  // enable pull-up resistor

	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf
	printString("Hello world!");
	transmitByte(0x0d);  //  cr
	transmitByte(0x0a);  // lf

	while (1) {
		if (((PIND & (1 << PD6)) == 0) || (getByte() == 'x')) {  // if you send an 'x' or hit the button
			// read the ADC (currently a photoresistor
			ADCSRA |= (1 << ADSC);  // start conversion
			loop_until_bit_is_clear(ADCSRA, ADSC);
			adcValue = ADC;

			// turn on the LEDS as a bar grph
			ledValue = (adcValue >> 7);

			LED_PORT = 0;
			for(i=0; i <= ledValue; i++) {
				LED_PORT |= (1 << i);
			}

			// log sensor reading to serial port
			printString("resistor reading: ");
			printByte(adcValue >> 2);
			printString(" (");
			printBinaryByte(ledValue);
			printString(") ");
			printString(" | ultrasonic: ");
			printWord(readUltrasonic());
			transmitByte(32);  //  sp
			printByte(ultrasonicInches());
			transmitByte(0x0d);  //  cr
			transmitByte(0x0a);  // lf

			//_delay_ms(500);
		}
	}
	return 0;
}
