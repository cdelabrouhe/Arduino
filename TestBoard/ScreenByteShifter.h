#pragma once

#include "arduino.h"

/*

2011-10-26

Paul Jenkins

This code is in the public domain.

//

Example code for driving a 4 digit common cathode 7-segment display with
a 74HC595 8-bit shift register and 4 NPN transistors.

The 74HC595 shift register attaches to pins 8, 11 and 12 of Arduino.
See Arduino ShiftOut tutorial for more info:
http://www.arduino.cc/en/Tutorial/ShiftOut

Arduino digital pins 4 to 7 drive the cathode for digits 1 through 4.
Digit 1 is the leftmost. Wire the emitter of the NPN transistor to ground.
Connect the Arduino digital pin to the base of the transistor via a
1 k-ohm resistor so as not to provide excessive base drive and damage
the transistor. Connect the collector of the transistor to the cathode of
the display.

Be sure to use current limiting resistors from the shift register outputs
Qa to Qf to the anode segments of the LED display. I used 220 ohm resistors.

*/

enum ValueType
{
	ValueType_NONE = -1,
	ValueType_Int,
	ValueType_Float,
	ValueType_COUNT
};

class ScreenByteShifter
{
public:
	void Init(const int _latchPin = 12, const int _clockPin = 13, const int _dataPin = 8, const int _disp1 = 5, const int _disp2 = 4, const int _disp3 = 3, const int _disp4 = 2);
	void Process();
	void SetInt(const int _val);
	void SetFloat(const float _val, const float _nbDecimals);

private:
	void DisplayInt(int _val, int _coma = -1);
	void DisplayFloat(const int _nbDecimals);
	void DisplayADigit(int _dispno, byte _digit2disp);
	void AllDispOff();

protected:
	ValueType	m_type;
	union
	{
		int		m_iValue;
		float	m_fValue;
	};

	int m_floatDecimals = 3;

	// Pin connected to latch pin (RCLK,12) of 74HC595
	int m_latchPin;
	// Pin connected to clock pin (SRCLK,11) of 74HC595
	int m_clockPin;
	// Pin connected to Data in (SER,14) of 74HC595
	int m_dataPin;
	// Pin connected to cathodes
	int m_disp1;  // Displays are numbered from left to right
	int m_disp2;  // as disp1 to disp4.
	int m_disp3;  // Scan displays quickly and use POV to display
	int m_disp4;  // 4 digits with only 1 shift register
};