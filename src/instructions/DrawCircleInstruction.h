#pragma once
#define DrawCircleInstruction_h
#include <instructions/Instruction.h>
#include "values/Value.h"
#include <vector>

class DrawCircleInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* radiusValue;
        Value* xValue;
        Value* yValue;
        void execute();
        DrawCircleInstruction();
        DrawCircleInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
