#pragma once
#define ListValue_h
#include <Arcadable.h>

template <typename T1, typename T2> 
class ListValue: public Value {
	public:
        ValueArrayValueTypePointer<T1> listValue;
        NumberValueTypePointer<NumberValueType> listIndex;

        T2 get();
        void set(T2 newValue);
		bool isTruthy();
        ListValue();
        ListValue(
            unsigned short ID,
            ValueArrayValueTypePointer<T1> listValue,
            NumberValueTypePointer<NumberValueType> listIndex
        );
};
