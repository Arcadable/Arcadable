#pragma once
#define SystemConfig_h

class SystemConfig {
    public:
        int screenWidth;
        int screenHeight;
        unsigned int minMillisPerFrame;
        int expandedProperties[3];
        SystemConfig(
            int screenWidth,
            int screenHeight,
            int minMillisPerFrame
        );
};
