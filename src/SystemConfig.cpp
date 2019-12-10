#include "SystemConfig.h"
#include "Arduino.h"

SystemConfig::SystemConfig(
    unsigned int screenWidth,
    unsigned int screenHeight,
    unsigned int minMillisPerFrame,
    bool layoutIsZigZag,
    unsigned int wireClock,
    unsigned int newGamePollingInterval,
    unsigned char eepromAddress,
    unsigned int eepromReadWriteBufferSize,
    std::vector<int> *inputPins
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->minMillisPerFrame = minMillisPerFrame;
    this->layoutIsZigZag = layoutIsZigZag;
    this->wireClock = wireClock;
    this->newGamePollingInterval = newGamePollingInterval;
    this->eepromAddress = eepromAddress;
    this->eepromReadWriteBufferSize = eepromReadWriteBufferSize;
    expandedProperties[0] = screenWidth;
    expandedProperties[1] = screenHeight;
    expandedProperties[2] = minMillisPerFrame;
    expandedProperties[3] = layoutIsZigZag;
    expandedProperties[4] = wireClock;
    expandedProperties[5] = newGamePollingInterval;
    expandedProperties[6] = eepromAddress;
    expandedProperties[7] = eepromReadWriteBufferSize;

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