#include "SystemConfig.h"
#include "Arduino.h"

SystemConfig::SystemConfig(
    unsigned int screenWidth,
    unsigned int screenHeight,
    unsigned int minMillisPerFrame,
    std::vector<int> *inputPins
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->minMillisPerFrame = minMillisPerFrame;
    expandedProperties[0] = screenWidth;
    expandedProperties[1] = screenHeight;
    expandedProperties[2] = minMillisPerFrame;

    for ( auto &input : *inputPins ) {
      inputValues.insert(std::pair<int, bool>(input, false)); 
      pinMode(input, INPUT);
    }
};

void SystemConfig::fetchInputValues() {
  for (auto &input : inputValues) {
    inputValues[input.first] = digitalRead(input.first);
	}
};