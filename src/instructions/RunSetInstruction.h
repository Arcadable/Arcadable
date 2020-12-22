#pragma once
#define RunSetInstruction_h
#include <instructions/Instruction.h>
#include <instructions/InstructionSet.h>
#include <vector>

class RunSetInstruction: public Instruction {
	public:
        unsigned short set;

        std::vector<Executable>* getExecutables(bool async);
        RunSetInstruction();
        RunSetInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);
};
