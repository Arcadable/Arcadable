#pragma once
#define InstructionSet_h
#include <vector>
#include <instructions/Instruction.h>
#include <Executable.h>

class InstructionSet {
    public:
        unsigned short size;
        std::vector<unsigned short> instructions;
        
        unsigned short ID;
        bool async;
        std::vector<Executable> executables;
        std::vector<Executable> empty;
        std::vector<Executable> singleUseless;

        std::vector<Executable>* getExecutables();
        InstructionSet();
        InstructionSet(
            unsigned short ID,
            unsigned short size,
            bool async
        );
        void init(std::vector<unsigned short> ids);
    private: 
        void _processAwaiting(std::vector<Executable>* executables, std::vector<Executable>* out);


};
