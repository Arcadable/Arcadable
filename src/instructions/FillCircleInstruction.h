#pragma once
#define FillCircleInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class FillCircleInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* radiusValue;
        Value* xValue;
        Value* yValue;
        FillCircleInstruction();
        FillCircleInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
