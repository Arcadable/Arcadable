#pragma once
#define ValueArrayValueTypePointer_h
#include <Arcadable.h>


template <typename T>
class ValueArrayValueTypePointer: public ValuePointer<T> {
    public:
        T getObject();
        std::vector<ValuePointer<Value>> getValue();
        ValueArrayValueTypePointer();
        ValueArrayValueTypePointer(
            unsigned short ID
        );
}