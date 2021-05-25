#pragma once
#define DrawTextInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class DrawTextInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* colorValue;
        Value* scaleValue;
        Value* textValue;
        Value* xValue;
        Value* yValue;
        DrawTextInstruction();
        DrawTextInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
