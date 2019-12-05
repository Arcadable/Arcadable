#pragma once
#define SystemConfig_h
#include "ArcadableLib.h"

class SystemConfig {
    public:
        unsigned int screenWidth;
        unsigned int screenHeight;
        unsigned int minMillisPerFrame;
        unsigned int expandedProperties[3];
        std::map<int, bool> inputValues;
        SystemConfig(
            unsigned int screenWidth,
            unsigned int screenHeight,
            unsigned int minMillisPerFrame,
            std::vector<int> *inputPins
        );

        void fetchInputValues();
};
