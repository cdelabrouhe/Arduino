#pragma once

#include "arduino.h"

class ColorLed
{
public:
	void Init(const byte _pinRed, const byte _pinGreen, const byte _pinBlue);
	void SetColor(const byte _red, const byte _green, const byte _blue);
	void SetColor(const float _red, const float _green, const float _blue);
	void Process();

private:
	byte m_pins[3];

	byte m_red;
	byte m_green;
	byte m_blue;
};