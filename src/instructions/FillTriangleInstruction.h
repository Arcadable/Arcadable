#pragma once
#define FillTriangleInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class FillTriangleInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        Value* x3Value;
        Value* y3Value;
        FillTriangleInstruction();
        FillTriangleInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
