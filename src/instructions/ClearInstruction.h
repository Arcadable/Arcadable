#pragma once
#define ClearInstruction_h
#include <instructions/Instruction.h>
#include <vector>
#include <Executable.h>

class ClearInstruction: public Instruction {
	public:

        ClearInstruction();
        ClearInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
