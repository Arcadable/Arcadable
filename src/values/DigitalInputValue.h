#pragma once
#define DigitalInputValue_h
#include <values/Value.h>
#include <vector>
class DigitalInputValue: public Value {
	public:
        unsigned short index;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        DigitalInputValue();
        DigitalInputValue(
            unsigned short ID,
            unsigned short index
        );
};
