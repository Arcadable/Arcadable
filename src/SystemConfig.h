#pragma once
#define SystemConfig_h
#include "ArcadableLib.h"

class SystemConfig {
    public:
        unsigned int screenWidth;
        unsigned int screenHeight;
        unsigned int minMillisPerFrame;
        bool layoutIsZigZag;
        unsigned int expandedProperties[4];
        std::map<int, bool> inputValues;
        SystemConfig(
            unsigned int screenWidth,
            unsigned int screenHeight,
            unsigned int minMillisPerFrame,
            bool layoutIsZigZag,
            std::vector<int> *inputPins
        );

        void fetchInputValues();
};
