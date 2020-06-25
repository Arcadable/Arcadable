#pragma once
#define DrawLineInstruction_h
#include <Arcadable.h>

class DrawLineInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> x1Value;
        NumberValueTypePointer<NumberValueType> y1Value;
        NumberValueTypePointer<NumberValueType> x2Value;
        NumberValueTypePointer<NumberValueType> y2Value;
        void execute();
        DrawLineInstruction();
        DrawLineInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> x1Value,
            NumberValueTypePointer<NumberValueType> y1Value,
            NumberValueTypePointer<NumberValueType> x2Value,
            NumberValueTypePointer<NumberValueType> y2Value
        );
};
