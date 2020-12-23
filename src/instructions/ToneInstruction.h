#pragma once
#define ToneInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>
#include <Executable.h>

class ToneInstruction: public Instruction {
	public:
        Value* volumeValue;
        Value* frequencyValue;
        Value* durationValue;

        ToneInstruction();
        ToneInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
