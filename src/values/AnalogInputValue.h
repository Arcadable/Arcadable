#pragma once
#define AnalogInputValue_h
#include <values/Value.h>
#include <vector>
class AnalogInputValue: public Value {
	public:
        unsigned short index;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);

        void init(std::vector<unsigned short> ids);
		bool isTruthy();
        AnalogInputValue();
        AnalogInputValue(
            unsigned short ID,
            unsigned short index
        );
};
