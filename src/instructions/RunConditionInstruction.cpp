#include "RunConditionInstruction.h"
#include "Arcadable.h"

RunConditionInstruction::RunConditionInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::RunCondition, await) {}
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

std::vector<Executable>* RunConditionInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        if (this->evaluationValue->isTruthy()) {
            return *Arcadable::getInstance()->instructionSets[this->successSet].getExecutables();
        } else if (this->_HasFail) {
            return *Arcadable::getInstance()->instructionSets[this->failSet].getExecutables();
        }
        return {};

    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}