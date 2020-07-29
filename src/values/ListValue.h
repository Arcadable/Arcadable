#pragma once
#define ListValue_h
#include <values/Value.h>
#include <vector>
#include <values/ListDeclaration.h>


class ListValue: public Value {
	public:
        ListDeclaration* listValue;
        Value* listIndex;

        double getNumber();
        std::vector<unsigned short>* getValueArray();
        void setNumber(double newValue);
        void setValueArray(std::vector<unsigned short> newValue);
        void init(std::vector<unsigned short> ids);
		bool isTruthy();
        ListValue();
        ListValue(
            unsigned short ID
        );
};
