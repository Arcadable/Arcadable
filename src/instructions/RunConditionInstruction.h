#pragma once
#define RunConditionInstruction_h
#include <Arcadable.h>

class RunConditionInstruction: public Instruction {
	public:
        ValuePointer<Value> evaluationValue;
        InstructionSetPointer successSet;
        InstructionSetPointer failSet;

        void execute();
        RunConditionInstruction();
        RunConditionInstruction(
            unsigned short ID,
            ValuePointer<Value> evaluationValue,
            InstructionSetPointer successSet,
            InstructionSetPointer failSet
        );
};
