#include "Arduino.h"
#include <Wire.h>

void setup()
{
	// Serial Window (debugging)
	Serial.begin(9600);

	// I2C Two Wire initialisation
	Wire.begin();

	// PCF8574N is 'reverse' logic inasmuch it SINKS current
	// so HIGH is OFF and LOW is ON (will we remember this?)
	// Turn OFF all pins by sending a high byte (1 bit per byte)
	Wire.beginTransmission(0x027);
	Wire.write(0xF);
	Wire.endTransmission();
}


void loop()
{
	// Addressing the PCF8574N via I2C is easy:
	/*
	 * 		I	7	6	5	4	3	2	1	0
	 * 		|	|	|	|	|	|	|	|	|
	 *		|	P7							P0
	 *	    Interrupt
	 *
	 *	   So to make P0 HIGH we set the bit to LOW (yes, confusing)
	 *	   eg 0b1111110 where 0b means we're specify a binary value
	 */

	//Simple LED blink! Turn ON P0 bit by setting LOW (to zero)
	Wire.beginTransmission(0x027);
	Wire.write(0b11111110);
	Wire.endTransmission();

	delay (500);

	// Now turn OFF P0 and turn P1 ON
	Wire.beginTransmission(0x027);
	Wire.write(0b11111101);
	Wire.endTransmission();

	delay (500);
}
