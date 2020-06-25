#pragma once
#define SetRotationInstruction_h
#include <Arcadable.h>

class SetRotationInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> rotationValue;

        void execute();
        SetRotationInstruction();
        SetRotationInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> rotationValue
        );
};
