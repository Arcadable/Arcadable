#include "SystemConfig.h"
#include "Arduino.h"

SystemConfig::SystemConfig(
    unsigned short int screenWidth,
    unsigned short int screenHeight,
    unsigned short int minMillisPerFrame,
    bool layoutIsZigZag,
    unsigned int wireClock,
    unsigned short int newGamePollingInterval,
    unsigned char eepromAddress,
    unsigned char eepromReadWriteBufferSize,
    unsigned char regDataWidth,
    unsigned char regShiftLoadPin,
    unsigned char regClockInihibitPin,
    unsigned char regSerialOutputPin,
    unsigned char regClockInputPin,
    unsigned char numAnalogInputs,
    unsigned short int analogInputPin,
    unsigned short int analogMaxValue,
    std::vector<unsigned char> *analogSignalPins
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->minMillisPerFrame = minMillisPerFrame;
    this->layoutIsZigZag = layoutIsZigZag;
    this->wireClock = wireClock;
    this->newGamePollingInterval = newGamePollingInterval;
    this->eepromAddress = eepromAddress;
    this->eepromReadWriteBufferSize = eepromReadWriteBufferSize;
    this->regDataWidth = regDataWidth;
    this->regShiftLoadPin = regShiftLoadPin;
    this->regClockInihibitPin = regClockInihibitPin;
    this->regSerialOutputPin = regSerialOutputPin;
    this->regClockInputPin = regClockInputPin;
    this->numAnalogInputs = numAnalogInputs;
    this->analogSignalPins = analogSignalPins;
    this->analogInputPin = analogInputPin;

    expandedProperties[0] = screenWidth;
    expandedProperties[1] = screenHeight;
    expandedProperties[2] = minMillisPerFrame;
    expandedProperties[3] = layoutIsZigZag;

    analogValueModifier = (1 << 21) / analogMaxValue;

    pinMode(this->regShiftLoadPin, OUTPUT);
    pinMode(this->regClockInihibitPin, OUTPUT);
    pinMode(this->regClockInputPin, OUTPUT);
    pinMode(this->regSerialOutputPin, INPUT);

    digitalWrite(this->regClockInputPin, LOW);
    digitalWrite(this->regShiftLoadPin, HIGH);
    for(unsigned char i = 0; i < this->regDataWidth ; i++) {
      digitalInputValues.insert(std::pair<unsigned char, bool>(i, false)); 
    }
    for(unsigned char i = 0; i < this->numAnalogInputs ; i++) {
      analogInputValues.insert(std::pair<unsigned char, unsigned short int>(i, 512)); 
    }

    for ( auto &pin : *this->analogSignalPins ) {
      pinMode(pin, OUTPUT);
    }
};

void SystemConfig::fetchInputValues() {
  digitalWrite(this->regClockInihibitPin, HIGH);
  digitalWrite(this->regShiftLoadPin, LOW);
  delayMicroseconds(5);
  digitalWrite(this->regShiftLoadPin, HIGH);
  digitalWrite(this->regClockInihibitPin, LOW);
  for(unsigned char i = 0; i < this->regDataWidth ; i++) {
    digitalInputValues[i] = digitalRead(this->regSerialOutputPin);
    if(i == 0) {
      Serial.println(digitalInputValues[i]);
    }
    digitalWrite(this->regClockInputPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(this->regClockInputPin, LOW);
  }


  for(unsigned char i = 0; i < this->numAnalogInputs ; i++) {
    unsigned char index = 0;
    for ( auto &pin : *this->analogSignalPins ) {
      digitalWrite(pin, (i >> index) & 0b1);
      index++;
    }
    analogInputValues[i] =(((unsigned int)analogRead(this->analogInputPin) << 11) * analogValueModifier) >> 22;

  }

};
