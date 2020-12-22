#pragma once
#define ClearInstruction_h
#include <instructions/Instruction.h>
#include <vector>

class ClearInstruction: public Instruction {
	public:

        std::vector<Executable>* getExecutables(bool async);
        ClearInstruction();
        ClearInstruction(
            unsigned short ID,
            bool await
        );
        void init(std::vector<unsigned short> ids);

};
