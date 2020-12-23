#pragma once
#define RunConditionInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <instructions/InstructionSet.h>
#include <vector>
#include <Executable.h>

class RunConditionInstruction: public Instruction {
	public:
        Value* evaluationValue;
        unsigned short successSet;
        unsigned short failSet;

        RunConditionInstruction();
        RunConditionInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

    private:
        bool _HasFail;
};
