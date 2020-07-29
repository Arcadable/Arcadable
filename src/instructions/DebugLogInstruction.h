#pragma once
#define DebugLogInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DebugLogInstruction: public Instruction {
	public:
        Value* logValue;

        void execute();
        DebugLogInstruction();
        DebugLogInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);


    private:
        void print(Value* v);
};
