#pragma once
#define InstructionSet_h
#include <vector>
#include <instructions/Instruction.h>

class InstructionSet {
    public:
        unsigned short size;
        std::vector<unsigned short> instructions;
        
        unsigned short ID;

        void execute();
        InstructionSet();
        InstructionSet(
            unsigned short ID,
            unsigned short size
        );
        void init(std::vector<unsigned short> ids);


};
