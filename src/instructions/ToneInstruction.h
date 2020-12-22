#pragma once
#define ToneInstruction_h
#include <instructions/Instruction.h>
#include <values/Value.h>
#include <vector>

class ToneInstruction: public Instruction {
	public:
        Value* volumeValue;
        Value* frequencyValue;
        Value* durationValue;

        std::vector<Executable>* getExecutables(bool async);
        ToneInstruction();
        ToneInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
