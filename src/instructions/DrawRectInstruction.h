#pragma once
#define DrawRectInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawRectInstruction: public Instruction {
	public:
        Value* colorValue;
        Value* x1Value;
        Value* y1Value;
        Value* x2Value;
        Value* y2Value;
        std::vector<Executable>* getExecutables(bool async);
        DrawRectInstruction();
        DrawRectInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
