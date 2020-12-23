#pragma once
#define WaitInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class WaitInstruction: public Instruction {
	public:
        Value* amountValue;

        WaitInstruction();
        WaitInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
