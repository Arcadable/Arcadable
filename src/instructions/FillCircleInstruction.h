#pragma once
#define FillCircleInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class FillCircleInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* colorValue;
        Value* radiusValue;
        Value* xValue;
        Value* yValue;
        FillCircleInstruction();
        FillCircleInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
