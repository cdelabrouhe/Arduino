#pragma once

#include "arduino.h"

class DistanceSensor
{
public:
	void Init(const byte _trigPin, const byte _echoPin);
	int Process(const float _temperature);

private:
	byte m_trigPin;
	byte m_echoPin;
};