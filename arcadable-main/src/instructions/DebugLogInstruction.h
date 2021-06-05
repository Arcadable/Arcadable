#pragma once
#define DebugLogInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
class GameState;

class DebugLogInstruction: public Instruction {
	public:
        GameState *game;
        Value* logValue;

        DebugLogInstruction();
        DebugLogInstruction(
            unsigned short ID,
            bool await,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

    private:
        void print(Value* v);

};
