/*
 * ultraSonic.c
 *
 *  Created on: Jun 17, 2015
 *      Author: jimparker
 */
#include "ultraSonic.h"
#include <avr/io.h>
#include <util/delay.h>

#define SONIC_PIN  7
#define SONIC_PORT D

uint16_t readUltrasonic(void) {
//    DDR#SONIC_PORT |= (1 << #SONIC_PIN);
	DDRD |= (1 << SONIC_PIN);              // set to output
	PORTD &= ~(1 << SONIC_PIN);            // clear pin
	_delay_us(2);                          // wait a bit
	PORTD |= (1 << SONIC_PIN);             // send a 5 microsecond pulse
	_delay_us(5);                          //
	PORTD &= ~(1 << SONIC_PIN);            // clear pin

	// TODO:  This method, I now know, stinks.  Need to start a timer and set an interrupt.
	//        I should be able to start a timer on SONIC_PIN set, and interrupt when clear.
	//        (I think...)
	//        something like start a microsecond timer, clear when SONIC_PORT goes high, INT when low

	DDRD &= ~(1 << SONIC_PIN);             // set to input
	while (~(PIND | ~(1 << SONIC_PIN)));   // wait while pin is clear

	uint16_t delay = 0;
	while (PIND & (1 << SONIC_PIN)) {      // while pin is set
		_delay_us(1);
		delay++;
	}
	return delay;
}

/* The measured distance from the range 0 to 400 cm */
uint16_t ultrasonicCentimeters(void) {
	return readUltrasonic()/29/2;
}

/* The measured distance from the range 0 to 157 Inches */
uint8_t ultrasonicInches(void) {
	uint16_t sonicValue = readUltrasonic();
	return sonicValue/74/2;
}

