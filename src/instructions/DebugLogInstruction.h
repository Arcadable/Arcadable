#pragma once
#define DebugLogInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class DebugLogInstruction: public Instruction {
	public:
        Value* logValue;

        DebugLogInstruction();
        DebugLogInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);


    private:
        void print(Value* v);
};
