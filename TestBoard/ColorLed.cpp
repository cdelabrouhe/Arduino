#include "ColorLed.h"

void ColorLed::Init(const byte _pinRed, const byte _pinGreen, const byte _pinBlue)
{
	m_pins[0] = _pinRed;
	m_pins[1] = _pinGreen;
	m_pins[2] = _pinBlue;

	pinMode(_pinRed, OUTPUT);
	pinMode(_pinGreen, OUTPUT);
	pinMode(_pinBlue, OUTPUT);
}

void ColorLed::SetColor(const byte _red, const byte _green, const byte _blue)
{
	m_red = _red;
	m_green = _blue;
	m_blue = _blue;
}

void ColorLed::SetColor(const float _red, const float _green, const float _blue)
{
	m_red = byte(_red * 255.0f);
	m_green = byte(_green * 255.0f);
	m_blue = byte(_blue * 255.0f);
}

void ColorLed::Process()
{
	analogWrite(m_pins[0], m_red);
	analogWrite(m_pins[1], m_green);
	analogWrite(m_pins[2], m_blue);
}