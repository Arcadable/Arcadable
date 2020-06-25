#pragma once
#define TextValue_h
#include <Arcadable.h>

template <typename T> 
class TextValue: public ValueArrayValueType {
	public:
        std::vector<ValuePointer<T>> values;

        std::vector<ValuePointer<T>> get();
        void set(std::vector<ValuePointer<T>> newValue);
		bool isTruthy();
        TextValue();
        TextValue(
            unsigned short ID,
            unsigned short size,
            std::vector<ValuePointer<T>> values
        );
};
