#pragma once
#define ListDeclaration_h
#include <Arcadable.h>

template <typename T> 
class ListDeclaration: public ValueArrayValueType {
	public:
        std::vector<ValuePointer<T>> values;

        std::vector<ValuePointer<T>> get();
        void set(std::vector<ValuePointer<T>> newValue);
		bool isTruthy();
        ListDeclaration();
        ListDeclaration(
            unsigned short ID,
            unsigned short size,
            std::vector<ValuePointer<T>> values
        );
};
