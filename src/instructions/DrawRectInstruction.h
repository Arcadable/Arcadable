#pragma once
#define DrawRectInstruction_h
#include <Arcadable.h>

class DrawRectInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> x1Value;
        NumberValueTypePointer<NumberValueType> y1Value;
        NumberValueTypePointer<NumberValueType> x2Value;
        NumberValueTypePointer<NumberValueType> y2Value;
        void execute();
        DrawRectInstruction();
        DrawRectInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> x1Value,
            NumberValueTypePointer<NumberValueType> y1Value,
            NumberValueTypePointer<NumberValueType> x2Value,
            NumberValueTypePointer<NumberValueType> y2Value
        );
};
