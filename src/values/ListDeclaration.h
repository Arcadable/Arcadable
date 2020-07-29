#pragma once
#define ListDeclaration_h
#include <vector>
#include <values/Value.h>

class ListDeclaration: public Value {
	public:
        unsigned short size;
        std::vector<unsigned short> values;
        std::vector<unsigned short> ids;
        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        ListDeclaration();
        ListDeclaration(
            unsigned short ID,
            unsigned short size
        );
};



