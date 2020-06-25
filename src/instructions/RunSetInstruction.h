#pragma once
#define RunSetInstruction_h
#include <Arcadable.h>

class RunSetInstruction: public Instruction {
	public:
        InstructionSetPointer set;

        void execute();
        RunSetInstruction();
        RunSetInstruction(
            unsigned short ID,
            InstructionSetPointer set
        );
};
