#include "RunSetInstruction.h"
#include "Arcadable.h"

RunSetInstruction::RunSetInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::RunSet, await) {
}
RunSetInstruction::RunSetInstruction() {}

void RunSetInstruction::init(std::vector<unsigned short> ids) {
    this->set = ids[0];
}
std::vector<Executable>* RunSetInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {
        return *Arcadable::getInstance()->instructionSets[this->set].getExecutables();
    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}