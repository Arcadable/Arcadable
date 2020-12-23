#pragma once
#define MutateValueInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class MutateValueInstruction: public Instruction {
	public:
        Value* leftValue;
        Value* rightValue;

        MutateValueInstruction();
        MutateValueInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

    private:
        bool isNumberType;
};
