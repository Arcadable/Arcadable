#pragma once
#define FillTriangleInstruction_h
#include <Arcadable.h>

class FillTriangleInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> x1Value;
        NumberValueTypePointer<NumberValueType> y1Value;
        NumberValueTypePointer<NumberValueType> x2Value;
        NumberValueTypePointer<NumberValueType> y2Value;
        NumberValueTypePointer<NumberValueType> x3Value;
        NumberValueTypePointer<NumberValueType> y3Value;
        void execute();
        FillTriangleInstruction();
        FillTriangleInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> x1Value,
            NumberValueTypePointer<NumberValueType> y1Value,
            NumberValueTypePointer<NumberValueType> x2Value,
            NumberValueTypePointer<NumberValueType> y2Value,
            NumberValueTypePointer<NumberValueType> x3Value,
            NumberValueTypePointer<NumberValueType> y3Value
        );
};
