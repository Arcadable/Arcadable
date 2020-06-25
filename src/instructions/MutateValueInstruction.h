#pragma once
#define MutateValueInstruction_h
#include <Arcadable.h>

class MutateValueInstruction: public Instruction {
	public:
        ValuePointer<Value> leftValue;
        ValuePointer<Value> rightValue;
        void execute();
        MutateValueInstruction();
        MutateValueInstruction(
            unsigned short ID,
            ValuePointer<Value> leftValue,
            ValuePointer<Value> rightValue
        );
};
