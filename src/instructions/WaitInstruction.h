#pragma once
#define WaitInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class WaitInstruction: public Instruction {
	public:
        Value* amountValue;

        std::vector<Executable>* getExecutables(bool async);
        WaitInstruction();
        WaitInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
