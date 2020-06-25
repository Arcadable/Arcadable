#pragma once
#define struction_h
#include <Arcadable.h>

enum structionType {
    MutateValue,
    RunCondition,
    DrawPixel,
    DrawLine,
    DrawRect,
    FillRect,
    DrawCircle,
    FillCircle,
    DrawTriangle,
    FillTriangle,
    DrawText,
    Clear,
    SetRotation,
	RunSet,
	DebugLog
};

class Instruction: public LogicElement {
    public:
        InstructionType type;
        
        virtual void execute();
        Instruction();
        Instruction(
            unsigned short ID,
            InstructionType type
        );

};

class InstructionPointer {
	public:
		unsigned short ID;
		Arcadable *game;
		void execute();

        InstructionPointer();
        InstructionPointer(
            unsigned short ID
        );
};
