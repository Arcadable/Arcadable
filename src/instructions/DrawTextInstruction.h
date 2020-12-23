#pragma once
#define DrawTextInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class DrawTextInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* scaleValue;
        Value* textValue;
        Value* xValue;
        Value* yValue;
        DrawTextInstruction();
        DrawTextInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
