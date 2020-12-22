#include "MutateValueInstruction.h"
#include <Arcadable.h>
MutateValueInstruction::MutateValueInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::MutateValue, await) { }
MutateValueInstruction::MutateValueInstruction() {}

void MutateValueInstruction::init(std::vector<unsigned short> ids) {
    this->leftValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->rightValue = Arcadable::getInstance()->values.find(ids[1])->second;
}

std::vector<Executable>* MutateValueInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        if(this->leftValue->isNumberType) {
            double right = this->rightValue->getNumber();
            this->leftValue->setNumber(right);
        } else {
            std::vector<unsigned short>* right = this->rightValue->getValueArray();
            this->leftValue->setValueArray(*right);
        }
        return {};

    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}