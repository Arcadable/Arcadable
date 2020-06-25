#pragma once
#define DebugLogInstruction_h
#include <Arcadable.h>

class DebugLogInstruction: public Instruction {
	public:
        ValuePointer<Value> logValue;

        void execute();
        DebugLogInstruction();
        DebugLogInstruction(
            unsigned short ID,
            ValuePointer<Value> logValue
        );
};
