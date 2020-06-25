#pragma once
#define InstructionSet_h
#include <Arcadable.h>

class InstructionSet: public LogicElement {
    public:
        InstructionType type;
        std::vector<InstructionPointer> instructions;

        void execute();
        InstructionSet();
        InstructionSet(
            unsigned short ID,
            unsigned short size,
            std::vector<InstructionPointer> instructions
        );

};

class InstructionSetPointer {
	public:
		unsigned short ID;
		Arcadable *game;
		void execute();

        InstructionSetPointer();
        InstructionSetPointer(
            unsigned short ID
        );
};
