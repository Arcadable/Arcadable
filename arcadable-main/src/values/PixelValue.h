#pragma once
#define PixelValue_h
#include "Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class PixelValue: public Value {
	public:
        DisplayRunner *display;
        GameState *game;
        Value* XCalc;
        Value* YCalc;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);
		bool isTruthy();
        void init(std::vector<unsigned short> ids);
        PixelValue();
        PixelValue(
            unsigned short ID,
            GameState *game,
            DisplayRunner *display
        );
};
