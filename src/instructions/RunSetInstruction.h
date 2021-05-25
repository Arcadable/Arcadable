#pragma once
#define RunSetInstruction_h
#include "Instruction.h"
#include "InstructionSet.h"
#include <vector>
class GameState;

class RunSetInstruction: public Instruction {
	public:
        GameState *game;
        unsigned short set;

        RunSetInstruction();
        RunSetInstruction(
            unsigned short ID,
            bool await,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
};
