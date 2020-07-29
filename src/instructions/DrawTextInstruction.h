#pragma once
#define DrawTextInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawTextInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* scaleValue;
        Value* textValue;
        Value* xValue;
        Value* yValue;
        void execute();
        DrawTextInstruction();
        DrawTextInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
