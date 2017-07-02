#include "DistanceSensor.h"
#include "HardwareSerial.h"

void DistanceSensor::Init(const byte _trigPin, const byte _echoPin)
{
	m_trigPin = _trigPin;
	m_echoPin = _echoPin;

	pinMode(m_trigPin, OUTPUT);
	pinMode(m_echoPin, INPUT);
}

int DistanceSensor::Process(const float _temperature)
{
	digitalWrite(m_trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(m_trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(m_trigPin, LOW);
	int duration = pulseIn(m_echoPin, HIGH);
	int distance = float(duration) / (2.0f * _temperature);

	/*Serial.print(distance);
	Serial.println(" cm");*/

	return distance;
}