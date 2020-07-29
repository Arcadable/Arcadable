#pragma once
#define ClearInstruction_h
#include <instructions/Instruction.h>
#include <vector>

class ClearInstruction: public Instruction {
	public:

        void execute();
        ClearInstruction();
        ClearInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
