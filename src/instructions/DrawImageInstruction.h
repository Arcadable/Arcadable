#pragma once
#define DrawImageInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class DrawImageInstruction: public Instruction {
	public:
        Value* xValue;
        Value* yValue;
        Value* imageValue;
        DrawImageInstruction();
        DrawImageInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
