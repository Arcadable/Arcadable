#pragma once
#define DataValue_h
#include <vector>
#include "Value.h"

class DataValue: public Value {
	public:
        unsigned short size;
        std::vector<short unsigned int> values;
        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<short unsigned int> newValue);
		bool isTruthy();
        void init(std::vector<short unsigned int> ids);
        DataValue();
        DataValue(
            unsigned short ID,
            unsigned short size
        );
};



