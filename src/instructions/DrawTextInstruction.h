#pragma once
#define DrawTextInstruction_h
#include <Arcadable.h>

class DrawTextInstruction: public Instruction {
	public:
        NumberValueTypePointer<NumberValueType> colorValue;
        NumberValueTypePointer<NumberValueType> scaleValue;
        ValueArrayValueTypePointer<TextValue<NumberValueType>> textValue;
        NumberValueTypePointer<NumberValueType> xValue;
        NumberValueTypePointer<NumberValueType> yValue;
        void execute();
        DrawTextInstruction();
        DrawTextInstruction(
            unsigned short ID,
            NumberValueTypePointer<NumberValueType> colorValue,
            NumberValueTypePointer<NumberValueType> scaleValue,
            ValueArrayValueTypePointer<TextValue<NumberValueType>> textValue,
            NumberValueTypePointer<NumberValueType> xValue,
            NumberValueTypePointer<NumberValueType> yValue
        );
};
