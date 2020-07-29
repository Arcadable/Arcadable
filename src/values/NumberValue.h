#pragma once
#define NumberValue_h
#include <values/Value.h>
#include <vector>
class NumberValue: public Value {
	public:
        double value;
        unsigned short size;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);

        NumberValue();
        NumberValue(
            unsigned short ID,
            double value,
            unsigned short size
        );
};
