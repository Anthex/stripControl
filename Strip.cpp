#include "Strip.h"
#include "Arduino.h"

Color::Color(int Red, int Green, int Blue)
{
  R = Red;
  G = Green;
  B = Blue;
}
Color::Color()
{
  R = 0;
  G = 0;
  B = 0;
}

/**
	Get red composite of a Color type
	@return red value as int from 0 to 255
*/
int Color::getRed()
{
  return R;
}
/**
	Get green composite of a Color type
	@return green value as int from 0 to 255
*/
int Color::getGreen()
{
  return G;
}
/**
	Get blue composite of a Color type
	@return blue value as int from 0 to 255
*/
int Color::getBlue()
{
  return B;
}
/**
	Changes color by RGB composites
	@param Red Red value [0-255]
	@param Green Green value [0-255]
	@param Blue Blue value [0-255]
*/
void Color::setColor(int Red, int Green, int Blue)
{
  R = Red;
  G = Green;
  B = Blue;
}



Strip::Strip()
{
  dataPin = NULL;
  clk = NULL;
}

Strip::Strip(int channel, int data_pin, int clock_pin)
{

  pinMode(data_pin, OUTPUT);
  pinMode(clock_pin, OUTPUT);
  dataPin = data_pin;
  clk = clock_pin;
  primaryColor = Color(0, 0, 10);
  light(30);
}

/**
	lights up the strip with solid color (primaryColor) up to a given LED number
	@param val the value up to which the strip must be lit
*/
void Strip::light(int val)
{
  sendStartFrame();

  // LIT LED FRAMES
  for (int a = 0; a < val; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    shiftOut(dataPin, clk, MSBFIRST, primaryColor.getBlue());
    shiftOut(dataPin, clk, MSBFIRST, primaryColor.getGreen());
    shiftOut(dataPin, clk, MSBFIRST, primaryColor.getRed());
  }

  //DIM LED FRAMES
  for (int a = val; a < STRIP_LENGTH; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    for (int i = 0; i < 3; i++)
    {
      shiftOut(dataPin, clk, MSBFIRST, 0x00);
    }
  }
}

/**
	lights up 3 LEDs in a solid color (primaryColor). The lit LEDs are the 3 consecutive LEDs starting at the given index (value)
	@param val the index at which the first LED will be lit
*/
void Strip::dot(int val, int len=3)
{
  sendStartFrame();

  for (int a = 0; a < val - len; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    shiftOut(dataPin, clk, MSBFIRST, 0);
    shiftOut(dataPin, clk, MSBFIRST, 0);
    shiftOut(dataPin, clk, MSBFIRST, 0);
  }

  for (int a = val - len; a < val; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getBlue() * (STRIP_LENGTH - a) + (secondaryColor.getBlue()*a))) / STRIP_LENGTH);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getGreen() * (STRIP_LENGTH - a) + (secondaryColor.getGreen()*a))) / STRIP_LENGTH);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getRed() * (STRIP_LENGTH - a) + (secondaryColor.getRed()*a))) / STRIP_LENGTH);
  }

  for (int a = val; a < STRIP_LENGTH; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    for (int i = 0; i < 3; i++)
    {
      shiftOut(dataPin, clk, MSBFIRST, 0x00);
    }
  }
}

/**
	Lights up the whole Strip in a solid color (primaryColor) at an intensity proportionnal to the input value
	@param val the intensity at which to light up the strip
*/
void Strip::pulse(int val)
{
  sendStartFrame();
  /*
    for (int a = 0; a < STRIP_LENGTH; a++) {
  	shiftOut(dataPin, clk, MSBFIRST, 0xff);
  	shiftOut(dataPin, clk, MSBFIRST, primaryColor.getBlue() / 5);
  	shiftOut(dataPin, clk, MSBFIRST, primaryColor.getGreen() / 5);
  	shiftOut(dataPin, clk, MSBFIRST, primaryColor.getRed() / 5);
    }
  */
}

/**
  lights up the strip with a gradient between the main and secondary colors up to a given LED number
  @param val the value up to which the strip must be lit
*/
void Strip::gradient(int val)
{
  sendStartFrame();
  // LIT LED FRAMES
  for (int a = 0; a < val; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getBlue() * (STRIP_LENGTH - a) + (secondaryColor.getBlue()*a))) / STRIP_LENGTH);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getGreen() * (STRIP_LENGTH - a) + (secondaryColor.getGreen()*a))) / STRIP_LENGTH);
    shiftOut(dataPin, clk, MSBFIRST, ((primaryColor.getRed() * (STRIP_LENGTH - a) + (secondaryColor.getRed()*a))) / STRIP_LENGTH);
  }

  //DIM LED FRAMES
  for (int a = val; a < STRIP_LENGTH; a++) {
    shiftOut(dataPin, clk, MSBFIRST, 0xff);
    for (int i = 0; i < 3; i++)
    {
      shiftOut(dataPin, clk, MSBFIRST, 0x00);
    }
  }
}

/**
	Sends a starting frame (32 bits of '0') which is required to reset the APA102C's index (similarly to a carriage return)
*/
void Strip::sendStartFrame()
{
  for (int i = 0; i < 4; i++) {
    shiftOut(dataPin, clk, MSBFIRST, 0x00);
  }
}

/**
	Sets the primary color of the strip
	@param col the new color
*/
void Strip::setPrimaryColor(Color col)
{
  primaryColor = col;
}

void Strip::setSecondaryColor(Color col)
{
  secondaryColor = col;
}

StereoStrip::StereoStrip()
{
  l = Strip();
  r = Strip();
}

StereoStrip::StereoStrip(Strip left, Strip right)
{
  l = left;
  r = right;
}

void StereoStrip::setPrimaryColor(Color col)
{
  l.setPrimaryColor(col);
  r.setPrimaryColor(col);
}

void StereoStrip::setSecondaryColor(Color col)
{
  l.setSecondaryColor(col);
  r.setSecondaryColor(col);
}

void StereoStrip::light(int val)
{
  l.light(val);
  r.light(val);
}

void StereoStrip::dot(int val)
{
  l.dot(val);
  r.dot(val);
}

void StereoStrip::pulse(int val)
{
  l.pulse(val);
  r.pulse(val);
}

void StereoStrip::gradient(int val)
{
  l.gradient(val);
  r.gradient(val);
}

void StereoStrip::update(int channel, byte value, byte mode)
{
  if (channel == LEFT) {
    switch (mode) {
      case 0: l.gradient(value);
        break;
      case 1: l.light(value);
        break;
      case 2:
        break;
      case 3: l.dot(value);
        break;
      case 4:
        break;
      case 5: l.pulse(value);
        break;
    }
  }
  else {
    switch (mode) {
      case 0: r.gradient(value);
        break;
      case 1: r.light(value);
        break;
      case 2:
        break;
      case 3: r.dot(value);
        break;
      case 4:
        break;
      case 5: r.pulse(value);
        break;
    }
  }
}

void StereoStrip::attach(Strip left, Strip right)
{
  l = left;
  r = right;
}
