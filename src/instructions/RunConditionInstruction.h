#pragma once
#define RunConditionInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <instructions/InstructionSet.h>
#include <vector>

class RunConditionInstruction: public Instruction {
	public:
        Value* evaluationValue;
        unsigned short successSet;
        unsigned short failSet;

        void execute();
        RunConditionInstruction();
        RunConditionInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

    private:
        bool _HasFail;
};
