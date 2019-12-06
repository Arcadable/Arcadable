#include "SystemConfig.h"
#include "Arduino.h"

SystemConfig::SystemConfig(
    unsigned int screenWidth,
    unsigned int screenHeight,
    unsigned int minMillisPerFrame,
    bool layoutIsZigZag,
    std::vector<int> *inputPins
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->minMillisPerFrame = minMillisPerFrame;
    this->layoutIsZigZag = layoutIsZigZag;
    expandedProperties[0] = screenWidth;
    expandedProperties[1] = screenHeight;
    expandedProperties[2] = minMillisPerFrame;
    expandedProperties[3] = layoutIsZigZag;

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