#pragma once
#define DrawPixelInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class DrawPixelInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* xValue;
        Value* yValue;
        DrawPixelInstruction();
        DrawPixelInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
