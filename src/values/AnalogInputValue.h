#pragma once
#define AnalogInputValue_h
#include <Arcadable.h>

class AnalogInputValue: public NumberValueType {
	public:
        unsigned short index;

        double get();
        void set(double newValue);
		bool isTruthy();
        AnalogInputValue();
        AnalogInputValue(
            unsigned short ID,
            unsigned short index;
        );
};
