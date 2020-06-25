#pragma once
#define ClearInstruction_h
#include <Arcadable.h>

class ClearInstruction: public Instruction {
	public:

        void execute();
        ClearInstruction();
        ClearInstruction(
            unsigned short ID
        );
};
