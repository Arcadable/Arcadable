#pragma once
#define Value_h
#include <Arcadable.h>

enum ValueType {

    number,
    pixelIndex,
    digitalInputPointer,
    analogInputPointer,
    systemPointer,
	listDeclaration,
	listValue,
    text,
    evaluation
};

class Value: public LogicElement {
    public:
        ValueType type;
        
        virtual template <typename T> T get();
        virtual template <typename T> void set(T newValue);
		virtual bool isTruthy();
        Value();
        Value(
            unsigned short ID,
            ValueType type
        );

};
