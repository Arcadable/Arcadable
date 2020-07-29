#pragma once
#define SystemConfigValue_h
#include <SystemConfig.h>
#include <values/Value.h>
#include <vector>
class SystemConfigValue: public Value {
	public:
        SystemConfigType configType;

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
