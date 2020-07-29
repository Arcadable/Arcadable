#pragma once
#define DrawLineInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawLineInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        void execute();
        DrawLineInstruction();
        DrawLineInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
