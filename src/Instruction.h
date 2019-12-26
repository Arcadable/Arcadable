#pragma once
#define Instruction_h
#include "ArcadableLib.h"
enum InstructionType { MutateValue, RunCondition };
class Instruction {

    public:
        unsigned short ID;
        Arcadable *game;
    
        unsigned short leftID;
        bool rightIsValue;
        unsigned short rightID;
        InstructionType instructionType; 

        Instruction();
        Instruction(
            unsigned short ID,
            unsigned short leftID,
            bool rightIsValue,
            unsigned short rightID,
            InstructionType instructionType
        );
        void execute();
};

