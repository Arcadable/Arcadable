#include "RunConditionInstruction.h"
#include "Arcadable.h"

RunConditionInstruction::RunConditionInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::RunCondition) {}
RunConditionInstruction::RunConditionInstruction() {}

void RunConditionInstruction::init(std::vector<unsigned short> ids) {
    this->evaluationValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->successSet = ids[1];

    if (ids.size() == 3) {
        this->failSet = ids[2];
        this->_HasFail = true;
    } else {
        this->_HasFail = false;
    }
}

void RunConditionInstruction::execute() {
    if (this->evaluationValue->isTruthy()) {
        Arcadable::getInstance()->instructionSets[this->successSet].execute();
    } else if (this->_HasFail) {
        Arcadable::getInstance()->instructionSets[this->failSet].execute();
    }
}