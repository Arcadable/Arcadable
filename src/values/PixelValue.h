#pragma once
#define PixelValue_h
#include <values/Value.h>
#include <vector>

class PixelValue: public Value {
	public:
        Value* XCalc;
        Value* YCalc;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        PixelValue();
        PixelValue(
            unsigned short ID
        );
};
