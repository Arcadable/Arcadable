#pragma once
#define InstructionSet_h
#include <vector>
#include <instructions/Instruction.h>

class InstructionSet {
    public:
        unsigned short size;
        std::vector<unsigned short> instructions;
        
        unsigned short ID;
        bool async;

        std::vector<Executable>* getExecutables();
        InstructionSet();
        InstructionSet(
            unsigned short ID,
            unsigned short size,
            bool async
        );
        void init(std::vector<unsigned short> ids);
    private: 
        std::vector<Executable>* processAwaiting(std::vector<Executable>& executables);


};
