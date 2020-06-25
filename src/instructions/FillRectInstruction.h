#pragma once
#define FillRectInstruction_h
#include <Arcadable.h>

class FillRectInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> x1Value;
        NumberValueTypePointer<NumberValueType> y1Value;
        NumberValueTypePointer<NumberValueType> x2Value;
        NumberValueTypePointer<NumberValueType> y2Value;
        void execute();
        FillRectInstruction();
        FillRectInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> x1Value,
            NumberValueTypePointer<NumberValueType> y1Value,
            NumberValueTypePointer<NumberValueType> x2Value,
            NumberValueTypePointer<NumberValueType> y2Value
        );
};
