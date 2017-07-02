#include "ScreenByteShifter.h"
#include "ColorLed.h"
#include "DistanceSensor.h"

enum Mode
{
	Mode_NONE = -1,
	Mode_Potentiometer,
	Mode_Light,
	Mode_Distance,
	Mode_Temperature,
	Mode_COUNT
};

ScreenByteShifter s_screen;
ColorLed s_LED;
DistanceSensor s_distanceSensor;

int s_potentiometerPin = A5;
int s_lightPin = A0;
int s_temperaturePin = A1;
int s_buttonPin = 6;

Mode s_mode = Mode(0);

void setup()
{
	Serial.begin(9600);

	s_screen.Init(12, 13, 10, 7, 5, 3, 2);
	s_LED.Init(6, 10, 5);
	s_distanceSensor.Init(8, 4);
	pinMode(s_buttonPin, INPUT);
}

void loop()
{
	float value = -1.0f;

	// Button state changes main mode
	static int buttonState = LOW;
	const int currentState = digitalRead(s_buttonPin);
	if ((currentState == HIGH) && (buttonState == LOW))
	{
		s_mode = Mode(s_mode + 1);
		if (s_mode == Mode_COUNT)
			s_mode = Mode(0);
	}
	buttonState = currentState;

	// Main mode
	switch (s_mode)
	{
		case Mode_Potentiometer:
			{
				const int potentiometer = analogRead(s_potentiometerPin);
				//s_screen.SetInt(potentiometer);
				value = (float)potentiometer / 1024.0f;
				s_screen.SetFloat(value, 3);
			}
			break;

		case Mode_Light:
			{
				const int light = analogRead(s_lightPin);
				value = (float)light / 1024.0f;
				s_screen.SetFloat(value, 3);
			}
			break;

		case Mode_Temperature:
			{
				//getting the voltage reading from the temperature sensor
				const int reading = analogRead(s_temperaturePin);

				// converting that reading to voltage, for 3.3v arduino use 3.3
				const float voltage = float(reading) * 3.3f / 1024.0f;

				// now print out the temperature
				const float temperature = (voltage /*- 0.5f*/) * 100.0f;  //converting from 10 mv per degree wit 500 mV offset
															 //to degrees ((voltage - 500mV) times 100)

				s_screen.SetFloat(temperature, 2);
				// Temperature doesn't make any sense => sensor probably broken
			}
			break;

		case Mode_Distance:
			{
				const int distance = s_distanceSensor.Process(29.1f);
				s_screen.SetInt(distance);
			}
			break;

		default:
			break;
	}

	// Led colour
	if (value >= 0.0f)
	{
		float red = max(0.0f, 0.5f - value) * 2.0f;
		float blue = max(0.0f, value - 0.5f) * 2.0f;
		float green = value < 0.5f ? 1.0f - red : 1.0f - blue;

		s_LED.SetColor(red, green, blue);
	}
	else
		s_LED.SetColor(byte(0), 0, 0);

	// Process LED
	s_LED.Process();

	// Process screen
	s_screen.Process();
}