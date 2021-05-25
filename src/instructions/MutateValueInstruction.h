#pragma once
#define MutateValueInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
class GameState;

class MutateValueInstruction: public Instruction {
	public:
        GameState *game;
        Value* leftValue;
        Value* rightValue;

        MutateValueInstruction();
        MutateValueInstruction(
            unsigned short ID,
            bool await,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
    private:
        bool isNumberType;

};
