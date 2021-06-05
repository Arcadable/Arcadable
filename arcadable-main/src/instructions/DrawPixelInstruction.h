#pragma once
#define DrawPixelInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class DrawPixelInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* colorValue;
        Value* xValue;
        Value* yValue;
        DrawPixelInstruction();
        DrawPixelInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
