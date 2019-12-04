#include "InputPins.h"
#include "Arduino.h"


InputPins::InputPins(
    int inputUpPin,
    int inputRightPin,
    int inputDownPin,
    int inputLeftPin
) {
    this->inputUpPin = inputUpPin;
    this->inputRightPin = inputRightPin;
    this->inputDownPin = inputDownPin;
    this->inputLeftPin = inputLeftPin;
    expandedProperties[0] = inputUpPin;
    expandedProperties[1] = inputRightPin;
    expandedProperties[2] = inputDownPin;
    expandedProperties[3] = inputLeftPin;
}

void InputPins::fetchInputValues() {
    unsigned char i = 0;
    for(int inputPin : expandedProperties) {
		inputValues[i] = digitalRead(inputPin);
        i++;
	}
}
