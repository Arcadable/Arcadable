#pragma once
#define Instruction_h
#include <vector>
#include <functional>

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

        std::vector<unsigned int> executables;



        std::vector<unsigned int>* getExecutables(bool async);
        virtual void init(std::vector<unsigned short> ids);
        virtual std::vector<unsigned int>* action(bool async);
        std::vector<unsigned int>* deepAction(bool async);



        Instruction();
        Instruction(
            unsigned short ID,
            InstructionType type,
            bool await
        );


};
