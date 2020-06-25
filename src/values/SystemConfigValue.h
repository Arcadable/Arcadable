#pragma once
#define SystemConfigValue_h
#include <Arcadable.h>

class SystemConfigValue: public NumberValueType {
	public:
        SystemConfigValue configType;

        double get();
        void set(double newValue);
		bool isTruthy();
        SystemConfigValue();
        SystemConfigValue(
            unsigned short ID,
            SystemConfigValue configType
        );
};
