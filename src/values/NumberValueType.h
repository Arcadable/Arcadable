#pragma once
#define NumberValueType_h
#include <Arcadable.h>

class NumberValueType: public Value {
    public:
        virtual double get();
        virtual void set(double newValue);
		virtual bool isTruthy();
};

template <typename T>
class NumberValueTypePointer: public ValuePointer<T> {
    public:
        T getObject();
        double getValue();
}