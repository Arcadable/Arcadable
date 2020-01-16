#pragma once
#define Instruction_h
#include "ArcadableLib.h"
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
    Clear
};
class Instruction {

    public:
        unsigned short ID;
        Arcadable *game;
    
        unsigned int leftID;
        bool rightIsValue;
        unsigned int rightID;
        InstructionType instructionType; 

        Instruction();
        Instruction(
            unsigned short ID,
            unsigned int leftID,
            bool rightIsValue,
            unsigned int rightID,
            InstructionType instructionType
        );
        void execute();
};

