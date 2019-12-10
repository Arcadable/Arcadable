#pragma once
#define SystemConfig_h
#include "ArcadableLib.h"

class SystemConfig {
    public:
        unsigned int screenWidth;
        unsigned int screenHeight;
        unsigned int minMillisPerFrame;
        bool layoutIsZigZag;
        unsigned int wireClock;
        unsigned int newGamePollingInterval;
        unsigned char eepromAddress;
        unsigned int eepromReadWriteBufferSize;
        unsigned int expandedProperties[8];
        std::map<int, bool> inputValues;
        SystemConfig(
            unsigned int screenWidth,
            unsigned int screenHeight,
            unsigned int minMillisPerFrame,
            bool layoutIsZigZag,
            unsigned int wireClock,
            unsigned int newGamePollingInterval,
            unsigned char eepromAddress,
            unsigned int eepromReadWriteBufferSize,
            std::vector<int> *inputPins
        );

        void fetchInputValues();
};
