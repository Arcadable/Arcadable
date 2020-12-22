#pragma once
#define DrawImageInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class DrawImageInstruction: public Instruction {
	public:
        Value* xValue;
        Value* yValue;
        Value* imageValue;
        std::vector<Executable>* getExecutables(bool async);
        DrawImageInstruction();
        DrawImageInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
