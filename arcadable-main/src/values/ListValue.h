#pragma once
#define ListValue_h
#include "Value.h"
#include <vector>
#include "ListDeclaration.h"
class GameState;


class ListValue: public Value {
	public:
        GameState *game;
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
            unsigned short ID,
            GameState *game
        );
};
