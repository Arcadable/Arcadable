#pragma once
#define Value_h
class ValuePointer;
#include <vector>
#include <values/Value.h>
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


class Value {
    public:
        ValueType type;
        unsigned short ID;
        bool isNumberType;
        std::vector<unsigned short> emptyArray;
        virtual double getNumber();
        virtual void setNumber(double newValue);
        virtual std::vector<unsigned short>* getValueArray();
        virtual void setValueArray(std::vector<unsigned short> newValue);
		virtual bool isTruthy();
        virtual void init(std::vector<unsigned short> ids);
        Value();
        Value(
            unsigned short ID,
            ValueType type
        );
};
