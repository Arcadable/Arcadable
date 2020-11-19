#pragma once
#define Instruction_h
#include <vector>

enum InstructionType {
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
	DebugLog,
    InstructionSetType,
    DrawImage
};

class Instruction {
    public:
        InstructionType type;
        unsigned short ID;
        
        virtual void execute();
        virtual void init(std::vector<unsigned short> ids);
        Instruction();
        Instruction(
            unsigned short ID,
            InstructionType type
        );

};
