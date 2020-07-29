#pragma once
#define MutateValueInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class MutateValueInstruction: public Instruction {
	public:
        Value* leftValue;
        Value* rightValue;

        void execute();
        MutateValueInstruction();
        MutateValueInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

    private:
        bool isNumberType;
};
