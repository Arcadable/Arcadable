#pragma once
#define ImageValue_h
#include "Value.h"
#include <vector>
class GameState;

class ImageValue: public Value {
	public:
        GameState *game;
        Value* data;
        Value* width;
        Value* height;
        Value* keyColor;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        ImageValue();
        ImageValue(
            unsigned short ID,
            GameState *game
        );
};
