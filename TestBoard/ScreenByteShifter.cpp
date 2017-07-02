#include "ScreenByteShifter.h"

/*

* Shift Register  Arduino
* 14  SER           11       Serial Input
* 11  SRCLK         12       Shift Clock
* 12  RCLK           8       Latch Enable

*/

// Define the bit-patterns for the 7-segment displays 
const byte SevenSeg[16] =
{
	// Hex digits
	B11111100, B01100000, B11011010, B11110010,   // 0123
	B01100110, B10110110, B10111110, B11100000,   // 4567
	B11111110, B11110110, B11101110, B00111110,   // 89AB
	B00011010, B01111010, B10011110, B10001110,   // CDEF

};

void ScreenByteShifter::Init(const int _latchPin, const int _clockPin, const int _dataPin, const int _disp1, const int _disp2, const int _disp3, const int _disp4)
{
	m_latchPin = _latchPin;
	m_clockPin = _clockPin;
	m_dataPin = _dataPin;
	m_disp1 = _disp1;
	m_disp2 = _disp2;
	m_disp3 = _disp3;
	m_disp4 = _disp4;

	pinMode(_latchPin, OUTPUT);
	pinMode(_clockPin, OUTPUT);
	pinMode(_dataPin, OUTPUT);
	pinMode(_disp1, OUTPUT);
	pinMode(_disp2, OUTPUT);
	pinMode(_disp3, OUTPUT);
	pinMode(_disp4, OUTPUT);
}

void ScreenByteShifter::Process()
{
	switch (m_type)
	{
	case ValueType_Int:
		DisplayInt(m_iValue);
		break;
	case ValueType_Float:
		DisplayFloat(m_floatDecimals);
		break;
	default:
		break;
	}
}

void ScreenByteShifter::SetInt(const int _val)
{
	m_type = ValueType_Int;
	m_iValue = _val;
}

void ScreenByteShifter::SetFloat(const float _val, const float _nbDecimals)
{
	m_type = ValueType_Float;
	m_fValue = _val;
	m_floatDecimals = _nbDecimals;
}

void ScreenByteShifter::DisplayADigit(int _dispno, byte _digit2disp)
{
	// Turn off the shift register pins
	// while you're shifting bits:
	digitalWrite(m_latchPin, LOW);

	AllDispOff();  // Turn off all cathode drivers.

				   // shift the bits out:
	shiftOut(m_dataPin, m_clockPin, LSBFIRST, ~_digit2disp);

	digitalWrite(m_latchPin, HIGH);  // Set latch high to set segments.

	digitalWrite(_dispno, HIGH);  // Drive one cathode low to turn on display.

	delay(5);  // Wait a bit for POV
}

void ScreenByteShifter::AllDispOff()
{
	// Turn all cathode drivers off
	digitalWrite(m_disp1, LOW);
	digitalWrite(m_disp2, LOW);
	digitalWrite(m_disp3, LOW);
	digitalWrite(m_disp4, LOW);
}

void ScreenByteShifter::DisplayInt(int _val, int _coma)
{
	int val = _val;

	int d1, d2, d3, d4; // Temporary values for thousands, hundreds, tens and units

	if (val > 9999)
		val = 9999;  // Do some bounds checks to avoid strangeness
	if (val < 0)
		val = 0;

	d1 = (int)(val / 1000);    // Get thousands
	val = val - (d1 * 1000);
	d2 = (int)(val / 100);     // Get hundreds
	val = val - (d2 * 100);
	d3 = (int)(val / 10);      // Get tens
	d4 = val - (d3 * 10);        // Get units

	DisplayADigit(m_disp1, byte(SevenSeg[d1]) + (_coma == 0 ? 1 : 0));  // Show thousands
	DisplayADigit(m_disp2, byte(SevenSeg[d2]) + (_coma == 1 ? 1 : 0));  // Show hundreds
	DisplayADigit(m_disp3, byte(SevenSeg[d3]) + (_coma == 2 ? 1 : 0));  // Show tens
	DisplayADigit(m_disp4, byte(SevenSeg[d4]) + (_coma == 3 ? 1 : 0));  // Show units
}

void ScreenByteShifter::DisplayFloat(const int _nbDecimals)
{
	float mult = 1.0f;
	for (int ID = 0; ID < _nbDecimals; ++ID)
		mult *= 10.0f;
	int val = m_fValue * mult;

	DisplayInt(val, 3 - _nbDecimals);
}