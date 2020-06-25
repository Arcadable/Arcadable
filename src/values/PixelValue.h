#pragma once
#define PixelValue_h
#include <Arcadable.h>

class PixelValue: public NumberValueType {
	public:
        NumberValueTypePointer<NumberValueType> XCalc;
        NumberValueTypePointer<NumberValueType> YCalc;

        double get();
        void set(double newValue);
		bool isTruthy();
        PixelValue();
        PixelValue(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> XCalc,
            NumberValueTypePointer<NumberValueType> YCalc
        );
};
