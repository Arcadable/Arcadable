#pragma once
#define RunConditionInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include "InstructionSet.h"
#include <vector>
class GameState;

class RunConditionInstruction: public Instruction {
	public:
        GameState *game;
        Value* evaluationValue;
        unsigned short successSet;
        unsigned short failSet;

        RunConditionInstruction();
        RunConditionInstruction(
            unsigned short ID,
            bool await,
            GameState *game
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
    private:
        bool _HasFail;

};
