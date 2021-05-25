#pragma once
#define ClearInstruction_h
#include "Instruction.h"
#include <vector>
#include "../displayRunner.h"

class ClearInstruction: public Instruction {
	public:

        DisplayRunner *display;

        ClearInstruction();
        ClearInstruction(
            unsigned short ID,
            bool await,
            DisplayRunner *display
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();

};
