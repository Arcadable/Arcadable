#pragma once
#define SystemConfigValue_h
#include "Value.h"
#include <vector>
#include "../configuration.h"
enum SystemConfigType {
    screenWidth,
    screenHeight,
    currentMillis,
    isZigZag
};
class SystemConfigValue: public Value {
	public:
        SystemConfigType configType;
        unsigned long startTime;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
        void init(std::vector<unsigned short> ids);

		bool isTruthy();
        SystemConfigValue();
        SystemConfigValue(
            unsigned short ID,
            SystemConfigType configType
        );
};
