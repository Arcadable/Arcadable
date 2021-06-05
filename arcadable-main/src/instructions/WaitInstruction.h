#pragma once
#define WaitInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
class GameState;


class WaitInstruction: public Instruction {
	public:
        GameState *game;
        Value* amountValue;

        WaitInstruction();
        WaitInstruction(
            unsigned short ID,
            bool await,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);

        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
