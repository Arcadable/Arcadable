#pragma once
#define ValueArrayValueType_h
#include <Arcadable.h>

class ValueArrayValueType: public Value {
    public:
        ValueType type;
        unsigned short size;

        virtual std::vector<ValuePointer<Value>> get();
        virtual void set(std::vector<ValuePointer<Value>> newValue);
		virtual bool isTruthy();

        ValueArrayValueType();
        ValueArrayValueType(
            unsigned short ID,
            ValueType type,
            unsigned short size
        );

};

template <typename T>
class ValueArrayValueTypePointer: public ValuePointer<T> {
    public:
        T getObject();
        std::vector<ValuePointer<Value>> getValue();
}