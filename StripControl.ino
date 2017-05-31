#include "Strip.h"

byte in; // incoming serial message
byte mode = 1; //see modes table, default = 1 (Plain color)

Strip leftColumn, rightColumn;
StereoStrip stereo;

void setup()
	
{
	Serial.begin(115200);
	leftColumn = Strip(LEFT, PC14, PC13);
	rightColumn = Strip(RIGHT, PB7, PB6);

	
	stereo.attach(leftColumn, rightColumn);
	stereo.setPrimaryColor(Color(0, 10, 0));	//GREEN
	stereo.setSecondaryColor(Color(0, 0, 10));	//RED
}

void loop()
{
	if (Serial.available() > 0)
	{
		in = Serial.read();
		switch (in)
		{
		case 125:
			stereo.setSecondaryColor(readColor());
			break;
		case 127:
			stereo.setPrimaryColor(readColor());
			break;
		case 126:
			mode = readMode();
			break;
		default:
			stereo.update((in>=64), in - 64 * (in >= 64), mode);
			break;
		}
	}
	
}

Color readColor() {
	while (Serial.available() < 3) {} //wait till R,G and B values are in the buffer
	return Color(Serial.read(), Serial.read(), Serial.read());
}

byte readMode() {
	while (Serial.available() < 1) {} //waiting for 1 parameter byte
	return Serial.read();
}

