#pragma once
#define DrawTriangleInstruction_h
#include <Arcadable.h>

class DrawTriangleInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> x1Value;
        NumberValueTypePointer<NumberValueType> y1Value;
        NumberValueTypePointer<NumberValueType> x2Value;
        NumberValueTypePointer<NumberValueType> y2Value;
        NumberValueTypePointer<NumberValueType> x3Value;
        NumberValueTypePointer<NumberValueType> y3Value;
        void execute();
        DrawTriangleInstruction();
        DrawTriangleInstruction(
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
