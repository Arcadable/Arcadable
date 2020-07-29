#include "RunSetInstruction.h"
#include "Arcadable.h"

RunSetInstruction::RunSetInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::RunSet) {
}
RunSetInstruction::RunSetInstruction() {}

void RunSetInstruction::init(std::vector<unsigned short> ids) {
    this->set = ids[0];
}
void RunSetInstruction::execute() {
    Arcadable::getInstance()->instructionSets[this->set].execute();
}