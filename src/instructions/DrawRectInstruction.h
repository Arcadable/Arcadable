#pragma once
#define DrawRectInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawRectInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        void execute();
        DrawRectInstruction();
        DrawRectInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
