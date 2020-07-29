#pragma once
#define SystemConfig_h
#include <map>
#include <vector>
#include "Arduino.h"

enum SystemConfigType {
    screenWidth,
    screenHeight,
    targetMainMillis,
    targetRenderMillis,
    currentMillis,
    isZigZag
};

class SystemConfig {
    public:
        unsigned short int screenWidth;
        unsigned short int screenHeight;
        unsigned short int targetMainMillis;
        unsigned short int targetRenderMillis;
        unsigned long int startMillis;
        bool layoutIsZigZag;
        unsigned int wireClock;
        unsigned short int newGamePollingInterval;
        unsigned char eepromAddress;
        unsigned char eepromReadWriteBufferSize;
        unsigned char regDataWidth;
        unsigned char regShiftLoadPin;
        unsigned char regClockInihibitPin;
        unsigned char regSerialOutputPin;
        unsigned char regClockInputPin;
        unsigned char numAnalogInputs;
        unsigned short int analogInputPin;
        unsigned short int analogMaxValue;
        unsigned int analogValueModifier;
        std::vector<unsigned char> *analogSignalPins;
        std::map<unsigned char, bool> digitalInputValues;
        std::map<unsigned char, unsigned short int> analogInputValues;
        SystemConfig();
        SystemConfig(
            unsigned short int screenWidth,
            unsigned short int screenHeight,
            unsigned short int targetMainMillis,
            unsigned short int targetRenderMillis,
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
            std::vector<unsigned char> *analogSignalPins
        );

        void fetchInputValues();

        double get(SystemConfigType type);
};
