#include "SystemConfig.h"

void SystemConfig::fetchInputValues() {
    digitalWrite(this->regClockInihibitPin, HIGH);
    digitalWrite(this->regShiftLoadPin, LOW);
    delayMicroseconds(5);
    digitalWrite(this->regShiftLoadPin, HIGH);
    digitalWrite(this->regClockInihibitPin, LOW);
   // Serial.println("Digital: ");

    for(unsigned char i = 0; i < this->regDataWidth ; i++) {
        digitalInputValues[i] = digitalRead(this->regSerialOutputPin);
        digitalWrite(this->regClockInputPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(this->regClockInputPin, LOW);
      //  Serial.print(i); Serial.print(": "); Serial.println(digitalInputValues[i]);
    }
    //Serial.println("Analog: ");

    for(unsigned char i = 0; i < this->numAnalogInputs ; i++) {
        unsigned char index = 0;
        for ( auto &pin : *this->analogSignalPins ) {
            digitalWrite(pin, (i >> index) & 0b1);
            index++;
        }
        unsigned short readValue = analogRead(this->analogInputPin);
        if(readValue > this->analogMaxValue) {
            analogInputValues[i] = 1023;
        } else {
            analogInputValues[i] =(((unsigned int)readValue << 11) * analogValueModifier) >> 22;
        }
      //  Serial.print(i); Serial.print(": "); Serial.println(analogInputValues[i]);

    }
  //  Serial.println("-----------------------------");
};

double SystemConfig::get(SystemConfigType type) {
    switch (type) {
        case SystemConfigType::screenWidth: {
            return this->screenWidth;
        }
        case SystemConfigType::screenHeight: {
            return this->screenHeight;
        }
        case SystemConfigType::targetMainMillis: {
            return this->targetMainMicros / 1000;
        }
        case SystemConfigType::targetRenderMillis: {
            return this->targetMainMicros / 1000;
        }
        case SystemConfigType::currentMillis: {
            return millis() - this->startMillis;
        }
        case SystemConfigType::isZigZag: {
            return this->layoutIsZigZag ? (double)1 : (double)0;
        }
        default: return 0;
    }
}

SystemConfig::SystemConfig(
    unsigned short int screenWidth,
    unsigned short int screenHeight,
    unsigned short int targetMainMicros,
    unsigned short int targetRenderMicros,
    unsigned long int startMillis,
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
    std::vector<unsigned char> *analogSignalPins,
    std::vector<unsigned char> *speakerOutputPins
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->targetMainMicros = targetMainMicros;
    this->targetRenderMicros = targetRenderMicros;
    this->startMillis = startMillis;
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
    this->speakerOutputPins = speakerOutputPins;
    this->analogInputPin = analogInputPin;
    this->analogMaxValue = analogMaxValue;

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
}
