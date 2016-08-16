#include "Arduino.h"
#include <Wire.h>

void setup() {
	// Serial Window (debugging)
	Serial.begin(9600);

	// I2C Two Wire initialisation
	Wire.begin();

	// PCF8574N is 'reverse' logic inasmuch it SINKS current
	// so HIGH is OFF and LOW is ON (will we remember this?)
	// Turn OFF all pins by sending a high byte (1 bit per byte)
	Wire.beginTransmission(0x27);
	Wire.write(0xF);
	Wire.endTransmission();

	Serial.println("Setup complete.");
}

void loop() {
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

	// Let's READ the status of the pins on the expander
	byte abc = 0;
	Wire.requestFrom(0x27, 1);
	if (Wire.available()) {
		abc = Wire.read();
	}

	// Print the binary version of the value (incl. leading zeros)
	printBinary(abc, 8);

	// If pin P7 is LOW then turn on the LED
	//otherwise switch if off
	if (abc & 0b10000000) {
		toggleLED(1);
	}
	else {
		toggleLED(0);
	}

	// Just for debugging
	delay(100);
}

void toggleLED(int status) {
	//Turn ON P0 bit by setting LOW (to zero)
	Wire.beginTransmission(0x27);
	if (status == 1) {
		Wire.write(0b11111110);
	}
	else {
		Wire.write(0b11111101);
	}
	Wire.endTransmission();
}

void printBinary(int v, int num_places) {
	int mask = 0, n;

	for (n = 1; n <= num_places; n++) {
		mask = (mask << 1) | 0x0001;
	}
	v = v & mask;  // truncate v to specified number of places

	while (num_places) {

		if (v & (0x0001 << (num_places - 1))) {
			Serial.print("1");
		}
		else {
			Serial.print("0");
		}

		--num_places;
		if (((num_places % 4) == 0) && (num_places != 0)) {
			Serial.print("_");
		}
	}
	Serial.println("");
}

