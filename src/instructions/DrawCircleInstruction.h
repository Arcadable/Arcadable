#pragma once
#define DrawCircleInstruction_h
#include <instructions/Instruction.h>
#include "values/Value.h"
#include <vector>
#include <Executable.h>

class DrawCircleInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* radiusValue;
        Value* xValue;
        Value* yValue;
        DrawCircleInstruction();
        DrawCircleInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
