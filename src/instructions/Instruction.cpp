#include "Instruction.h"
#include "../executable.h"

Instruction::Instruction (
    unsigned short ID,
    InstructionType type,
    bool await
) {
    this->type = type;
    this->ID = ID;
    this->await = await;
    this->executables = {};

}
Instruction::Instruction () { }

std::vector<unsigned int>* Instruction::getExecutables(bool async) {
    this->executables.clear();
    this->executables.push_back(Executable::createExecutable(this->ID, async, false, 4294967295, 65535, false));
    return &this->executables;
}
void Instruction::init(std::vector<unsigned short> ids) {}
std::vector<unsigned int>* Instruction::action(bool async) {
    return &Executable::empty;
}
std::vector<unsigned int>* Instruction::deepAction(bool async) {
    return this->getExecutables(async);
}
