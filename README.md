# PCF8574-Pin-Extender-I2C
Too few pins on your Arduino? Use this I2C controlled pin extender (with optional interrupt) to make life easier!

This supports my YouTube channel https://www.youtube.com/c/RalphBacon, and specifically video #49 PCF8574 Pin Extender.

There are three sketches here, starting with the simplest one, an LED flasher. Well, an LED alternater might be a better description and just ensures you have wired it all up correctly (only four wires: two for power and the two I2C SDA, SCL). I then outputs via I2C a HIGH (value 0 - different to Arduino) to turn ON the LED.

If the above statement seemed a bit odd, remember that the PCD8574 will SINK the current, not source it, so we have to make the pin there LOW to let current rush in and thereby light up the LED (or turn on the relay, whatever you have connected).

The second sketch shows some INPUT use as well as OUTPUT so you can detect switches being pressed. Here the ultimate lesson is to understand that although you send a READ request of 1 byte to the PCF8574 and you get back all 8 bits indicating which pins are currently LOW, the ones you have used as OUTPUTs previously should be ignored (they value returned contains no meaning, it might be a 0, it might be a 1).

Finally, the third sketch shows how we can use the interrupt pin on the PCF8574 to drive the interrupt pin on the Arduino LOW or HIGH to signal that one or more pins have now changed value (eg a switch has been pressed or released). Using interrupts is optional as you can just scan (poll) the PCF8754 for changes but it's not very efficient to do that if you are also trying to do 'other stuff' with your Arduino.

We don't use any libraries (although plenty exist for the PCF8574) as it's more useful to understand what it going on. The only library we require is the standard Wire library for the I2C use.

Finally, here's a link to some very useful print routines, although the one we're interested in here is for printing binrary values to the Serial window:
http://www.phanderson.com/arduino/arduino_display.html 

Any queries or comments, just put them at the bottom of the video and I'll do my best to answer!

Once again, thanks for watching and please do spread the word, give me a thumbs up if you like the video and don't forget to subscribe to get nofication of further (just about weekly) videos going forward!
