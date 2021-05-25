#pragma once
#define AnalogInputValue_h
#include "Value.h"
#include <vector>
#include "../analogReader.h"
class AnalogInputValue: public Value {
	public:
        AnalogReader *reader;
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
            unsigned short index,
            AnalogReader *reader
        );
};
