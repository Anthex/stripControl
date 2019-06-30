#include <EEPROM.h>
#include "Strip.h"

byte in; // incoming serial message
byte mode = 1; //see modes table, default = 1 (Plain color)

Strip leftColumn, rightColumn;
StereoStrip stereo;

void setup()
	
{
	Serial1.begin(115200);
	leftColumn = Strip(LEFT, PA6, PA7);
	rightColumn = Strip(RIGHT, PA1, PA2);

	
	stereo.attach(leftColumn, rightColumn);
	//stereo.setPrimaryColor(Color(0, 10, 0));	//GREEN
	//stereo.setSecondaryColor(Color(0, 0, 10));	//RED

  Color defaultColor1 = Color(100,0,100);
  Color defaultColor2 = Color(0,100,100);
  stereo.setPrimaryColor(fromEEPROM(0));
  stereo.setSecondaryColor(fromEEPROM(1));
    
  //stereo.setPrimaryColor(Color(0,10,0));
  stereo.light(10);

 int index = 0;
 int dir = 0; 
 while(!Serial1.available()){
  index++;
  if (index < 61){
    (1)? leftColumn.dot(index,1) : leftColumn.dot(60-index,1);
    (!1)? rightColumn.dot(index,1) : rightColumn.dot(60-index,1);
  }else if (index < 121){
    (1)? rightColumn.dot(index-60,1) : rightColumn.dot(60-index+60,1);
    (!1)? leftColumn.dot(index-60,1) : leftColumn.dot(60-index+60,1);
  }
  if(index == 61){
    (!dir)? leftColumn.setPrimaryColor(defaultColor1) : leftColumn.setPrimaryColor(defaultColor2);
    (!dir)? rightColumn.setPrimaryColor(defaultColor2) : rightColumn.setPrimaryColor(defaultColor1);
  }
  if (index > 120){
     dir= !dir;
    (dir)? leftColumn.setPrimaryColor(defaultColor1) : leftColumn.setPrimaryColor(defaultColor2);
    (dir)? rightColumn.setPrimaryColor(defaultColor2) : rightColumn.setPrimaryColor(defaultColor1);
    index = 0;
  }
  delay(15);
 }
  mode = EEPROM.read(6);
  stereo.setPrimaryColor(fromEEPROM(0));
  stereo.setSecondaryColor(fromEEPROM(1));
}

void loop()
{
	if (Serial1.available() > 0)
	{
		in = Serial1.read();
		switch (in)
		{
		case SETSECONDARYCOLOR:
			stereo.setSecondaryColor(readColor(1));
			break;
		case SETPRIMARYCOLOR:
			stereo.setPrimaryColor(readColor(0));
			break;
		case READMODE:
			mode = readMode();
      EEPROM.write(6, mode);
			break;
		default:
			stereo.update((in>=64), in - 64 * (in >= 64), mode);
			break;
		}
	}
}

Color readColor(int id){
	while (Serial1.available() < 3) {} //wait till R,G and B values are in the buffer
  Color incoming = Color(Serial1.read(), Serial1.read(), Serial1.read());
  EEPROM.write(id*3, incoming.getRed());
  EEPROM.write(id*3+1, incoming.getGreen());
  EEPROM.write(id*3+2, incoming.getBlue());
	return incoming;
}

byte readMode() {
	while (Serial1.available() < 1) {} //waiting for 1 parameter byte
	return Serial1.read();
}

Color fromEEPROM(int id){
  return Color(EEPROM.read(id*3),EEPROM.read(id*3+1),EEPROM.read(id*3+2));
}

