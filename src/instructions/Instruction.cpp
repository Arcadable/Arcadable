#include "Instruction.h"
#include "Arcadable.h"

Instruction::Instruction (
    unsigned short ID,
    InstructionType type,
    bool await
) {
    this->type = type;
    this->ID = ID;
    this->await = await;
}
Instruction::Instruction () { }

std::vector<Executable>* Instruction::getExecutables(bool async) {}
void Instruction::init(std::vector<unsigned short> ids) {}