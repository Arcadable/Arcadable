#pragma once
#define SetRotationInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../displayRunner.h"
class GameState;

class SetRotationInstruction: public Instruction {
	public:
        GameState *game;
        DisplayRunner *display;
        Value* rotationValue;

        SetRotationInstruction();
        SetRotationInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
};
