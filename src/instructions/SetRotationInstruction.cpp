#include "SetRotationInstruction.h"
#include "Arcadable.h"

SetRotationInstruction::SetRotationInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::SetRotation, await) {
}
SetRotationInstruction::SetRotationInstruction() {}

void SetRotationInstruction::init(std::vector<unsigned short> ids) {
    this->rotationValue = Arcadable::getInstance()->values.find(ids[0])->second;
}

std::vector<Executable>* SetRotationInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        Arcadable::getInstance()->canvas->setRotation(static_cast<int>(this->rotationValue->getNumber()));
        return {};

    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}