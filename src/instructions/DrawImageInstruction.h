#pragma once
#define DrawImageInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawImageInstruction: public Instruction {
	public:
        Value* xValue;
        Value* yValue;
        Value* imageValue;
        void execute();
        DrawImageInstruction();
        DrawImageInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
