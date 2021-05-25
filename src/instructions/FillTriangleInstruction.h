#pragma once
#define FillTriangleInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class FillTriangleInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        Value* x3Value;
        Value* y3Value;
        FillTriangleInstruction();
        FillTriangleInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
};
