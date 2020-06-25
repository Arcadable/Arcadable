#pragma once
#define NumberValue_h
#include <Arcadable.h>

class NumberValue: public NumberValueType {
	public:
        double value;
        unsigned short size;

        double get();
        void set(double newValue);
		bool isTruthy();
        NumberValue();
        NumberValue(
            unsigned short ID,
            double value,
            unsigned short size
        );
};
