#include "Arduino.h"
#include <Wire.h>

// ISR flags that detects whether a switch was pressed
volatile bool status = true;
bool ISR_change = false;

// Prevent the ISR triggering on a switch bounce
unsigned long prevMillis = millis();

//-------------------------------------------------------------------------
// SETUP     SETUP     SETUP     SETUP     SETUP     SETUP     SETUP
//-------------------------------------------------------------------------
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

	// Interrupt routine (connect INT on PCF8574 to Pin 2 on Arduino)
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(2), ISRoutine, CHANGE);

	Serial.println("Setup complete.");
}

//-------------------------------------------------------------------------
// LOOP      LOOP     LOOP     LOOP     LOOP     LOOP     LOOP     LOOP
//-------------------------------------------------------------------------
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

	// Has there been a key press (notified by ISR which in turn updates
	// this boolean flag?
	if (ISR_change) {

		// Reset the flag ready for next time
		ISR_change = false;

		// Let's READ the status of the pins on the expander
		byte abc = 0;
		Wire.requestFrom(0x27, 1);
		if (Wire.available()) {
			abc = Wire.read();
		}

		// Determine whether P7 was LOW (value=1) by binary ADD against known value
		status = (abc & 0b10000000);

		// Debug window print value
		Serial.println(status);

		// Display LED depending on global flag (set by interrupt routine)
		toggleLED(status);
	}

	// Just for debugging
	delay(100);
}

//-------------------------------------------------------------------------
// Interrupt Service Routine to set a flag to show something happened
//-------------------------------------------------------------------------
void ISRoutine() {

	// Only call this max once every 1/4 second
	if (millis() > prevMillis + 250) {
		Serial.println("ISR called");
		Serial.flush();
		ISR_change = true;

		// Update the time so that we don't re-trigger within 1/4 second
		prevMillis = millis();
	}
}

//-------------------------------------------------------------------------
// Turn on various pins on the PCF8574 (which in turn control the LEDs)
//-------------------------------------------------------------------------
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

//-------------------------------------------------------------------------
// http://www.phanderson.com/arduino/arduino_display.html
//
// This has several useful print routines for the Arduino, worth a look
//-------------------------------------------------------------------------
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
