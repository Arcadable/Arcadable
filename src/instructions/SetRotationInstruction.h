#pragma once
#define SetRotationInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class SetRotationInstruction: public Instruction {
	public:
        Value* rotationValue;

        std::vector<Executable>* getExecutables(bool async);
        SetRotationInstruction();
        SetRotationInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);
};
