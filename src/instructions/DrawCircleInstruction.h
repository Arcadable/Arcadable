#pragma once
#define DrawCircleInstruction_h
#include <Arcadable.h>

class DrawCircleInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> radiusValue;
        NumberValueTypePointer<NumberValueType> xValue;
        NumberValueTypePointer<NumberValueType> yValue;
        void execute();
        DrawCircleInstruction();
        DrawCircleInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> radiusValue,
            NumberValueTypePointer<NumberValueType> xValue,
            NumberValueTypePointer<NumberValueType> yValue
        );
};
