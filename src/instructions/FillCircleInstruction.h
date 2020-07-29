#pragma once
#define FillCircleInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class FillCircleInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* radiusValue;
        Value* xValue;
        Value* yValue;
        void execute();
        FillCircleInstruction();
        FillCircleInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);

};
