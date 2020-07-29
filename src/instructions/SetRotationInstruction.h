#pragma once
#define SetRotationInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class SetRotationInstruction: public Instruction {
	public:
        Value* rotationValue;

        void execute();
        SetRotationInstruction();
        SetRotationInstruction(
            unsigned short ID
        );
        void init(std::vector<unsigned short> ids);
};
