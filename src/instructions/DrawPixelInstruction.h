#pragma once
#define DrawPixelInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawPixelInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* xValue;
        Value* yValue;
        void execute();
        DrawPixelInstruction();
        DrawPixelInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
