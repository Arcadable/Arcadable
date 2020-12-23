#pragma once
#define Instruction_h
#include <vector>
#include <Executable.h>

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
    DrawImage,
    Wait,
    Tone,
    AwaitedRunSet,
    AwaitedRunCondition,
    AwaitedTone
};

class Instruction {
    public:
        InstructionType type;
        unsigned short ID;
        bool await;

       // std::vector<Executable> awaiting;
        std::vector<Executable> executables;
        std::vector<Executable> empty;

        std::vector<Executable>* getExecutables(bool async);
        virtual void init(std::vector<unsigned short> ids);



        Instruction();
        Instruction(
            unsigned short ID,
            InstructionType type,
            bool await
        );

};
