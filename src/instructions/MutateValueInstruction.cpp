#include "MutateValueInstruction.h"
#include <Arcadable.h>
MutateValueInstruction::MutateValueInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::MutateValue) { }
MutateValueInstruction::MutateValueInstruction() {}

void MutateValueInstruction::init(std::vector<unsigned short> ids) {
    this->leftValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->rightValue = Arcadable::getInstance()->values.find(ids[1])->second;
}

void MutateValueInstruction::execute() {
    if(this->leftValue->isNumberType) {
        double right = this->rightValue->getNumber();
        this->leftValue->setNumber(right);
    } else {
        std::vector<unsigned short>* right = this->rightValue->getValueArray();
        this->leftValue->setValueArray(*right);
    }
}