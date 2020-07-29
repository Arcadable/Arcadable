#pragma once
#define RunSetInstruction_h
#include <instructions/Instruction.h>
#include <instructions/InstructionSet.h>
#include <vector>

class RunSetInstruction: public Instruction {
	public:
        unsigned short set;

        void execute();
        RunSetInstruction();
        RunSetInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);
};
