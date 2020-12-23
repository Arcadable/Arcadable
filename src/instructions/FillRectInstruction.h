#pragma once
#define FillRectInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class FillRectInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        FillRectInstruction();
        FillRectInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
