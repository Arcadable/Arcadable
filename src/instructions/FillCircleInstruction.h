#pragma once
#define FillCircleInstruction_h
#include <Arcadable.h>

class FillCircleInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> radiusValue;
        NumberValueTypePointer<NumberValueType> xValue;
        NumberValueTypePointer<NumberValueType> yValue;
        void execute();
        FillCircleInstruction();
        FillCircleInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> radiusValue,
            NumberValueTypePointer<NumberValueType> xValue,
            NumberValueTypePointer<NumberValueType> yValue
        );
};
