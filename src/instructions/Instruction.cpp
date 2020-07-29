#include "Instruction.h"
#include "Arcadable.h"

Instruction::Instruction (
    unsigned short ID,
    InstructionType type
) {
    this->type = type;
    this->ID = ID;
}
Instruction::Instruction () { }

void Instruction::execute() {}
void Instruction::init(std::vector<unsigned short> ids) {}