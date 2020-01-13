#pragma once
#define SystemConfig_h
#include "ArcadableLib.h"

class SystemConfig {
    public:
        unsigned short int screenWidth;
        unsigned short int screenHeight;
        unsigned short int minMillisPerFrame;
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
        std::vector<unsigned char> *analogSignalPins;
        unsigned int expandedProperties[4];
        std::map<unsigned char, bool> digitalInputValues;
        std::map<unsigned char, unsigned short int> analogInputValues;
        SystemConfig(
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
            std::vector<unsigned char> *analogSignalPins
        );

        void fetchInputValues();
};
