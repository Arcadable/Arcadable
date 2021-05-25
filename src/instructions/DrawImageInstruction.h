#pragma once
#define DrawImageInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class DrawImageInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* xValue;
        Value* yValue;
        Value* imageValue;
        DrawImageInstruction();
        DrawImageInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
