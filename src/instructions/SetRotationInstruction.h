#pragma once
#define SetRotationInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class SetRotationInstruction: public Instruction {
	public:
        Value* rotationValue;

        SetRotationInstruction();
        SetRotationInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);
};
