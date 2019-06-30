/**
  APA102 Serial VUmeter
  Strip.h : Handles functions needed to Control separate LED Strips
  @author Achille DANTZ
  @version 0.1 1/15/17
  TODO:
  - nonlinear color remapping
  - 6+ modes implementation
  - "pulse" mode reimplementation
  - idle animations
  NOTE : The MCU used for this project is an STM32F103C8. The native SPI interface is not used
  but the data and clock lines are bitbanged. Performance tests have shown 8-bit ATmega chips to be 
  too sluggish for an optimal refresh rate and have thus been replaced with a 32-bit alternative
*/

#define STRIP_LENGTH 60
#define LEFT 0
#define RIGHT 1

//Serial messages (range 128 - 255)
#define SETSECONDARYCOLOR 125
#define SETPRIMARYCOLOR 127
#define READMODE 126

#ifndef _STRIP_h
#define _STRIP_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Color
{
public:
	Color(int Red, int Green, int Blue);
	Color();
	int getRed();
	int getGreen();
	int getBlue();
	void setColor(int Red, int Green, int Blue);
private:
	int R, G, B;
};


class Strip
{
public:
	Strip();
	Strip(int channel, int data_pin, int clock_pin);
	void light(int val);
	void dot(int val,int len);
	void pulse(int val);
	void gradient(int val);
	void sendStartFrame();
	void setPrimaryColor(Color col);
	void setSecondaryColor(Color col);
private:
	int dataPin;
	int clk;
	Color primaryColor;
	Color secondaryColor;
};

#endif

class StereoStrip {
public:
	StereoStrip();
	StereoStrip(Strip left, Strip right);
	void setPrimaryColor(Color col);
	void setSecondaryColor(Color col);
	void light(int val);
	void dot(int val);
	void pulse(int val);
	void gradient(int val);
	void update(int channel, byte value, byte mode);
	void attach(Strip left, Strip right);
private:
	Strip l;
	Strip r;
};
