#include "SystemConfig.h"
#include "Arduino.h"

SystemConfig::SystemConfig(
    int screenWidth,
    int screenHeight,
    int minMillisPerFrame
) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->minMillisPerFrame = minMillisPerFrame;
    expandedProperties[0] = screenWidth;
    expandedProperties[1] = screenHeight;
    expandedProperties[2] = minMillisPerFrame;
};
