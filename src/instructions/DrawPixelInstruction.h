#pragma once
#define DrawPixelInstruction_h
#include <Arcadable.h>

class DrawPixelInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> xValue;
        NumberValueTypePointer<NumberValueType> yValue;
        void execute();
        DrawPixelInstruction();
        DrawPixelInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> xValue,
            NumberValueTypePointer<NumberValueType> yValue
        );
};
