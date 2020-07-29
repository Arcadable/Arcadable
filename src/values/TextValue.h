#pragma once
#define TextValue_h
#include <values/Value.h>
#include <vector>


class TextValue: public Value {
	public:
        unsigned short size;
        std::vector<unsigned short> values;
        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        TextValue();
        TextValue(
            unsigned short ID,
            unsigned short size
        );
};
