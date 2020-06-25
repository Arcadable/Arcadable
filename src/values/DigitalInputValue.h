#pragma once
#define DigitalInputValue_h
#include <Arcadable.h>

class DigitalInputValue: public NumberValueType {
	public:
        unsigned short index;

        double get();
        void set(double newValue);
		bool isTruthy();
        DigitalInputValue();
        DigitalInputValue(
            unsigned short ID,
            unsigned short index;
        );
};
