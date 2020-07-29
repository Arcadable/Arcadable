#include "SetRotationInstruction.h"
#include "Arcadable.h"

SetRotationInstruction::SetRotationInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::SetRotation) {
}
SetRotationInstruction::SetRotationInstruction() {}

void SetRotationInstruction::init(std::vector<unsigned short> ids) {
    this->rotationValue = Arcadable::getInstance()->values.find(ids[0])->second;
}

void SetRotationInstruction::execute() {
    Arcadable::getInstance()->canvas->setRotation(static_cast<int>(this->rotationValue->getNumber()));

}