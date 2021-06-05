#include "RunConditionInstruction.h"
#include "../executable.h"
#include "../gameState.h"

RunConditionInstruction::RunConditionInstruction (
    unsigned short ID,
    bool await,
    GameState *game
) : Instruction(ID, InstructionType::RunCondition, await) {
    this->game = game;
}
RunConditionInstruction::RunConditionInstruction() {}

void RunConditionInstruction::init(std::vector<unsigned short> ids) {
    this->evaluationValue = this->game->values.find(ids[0])->second;
    this->successSet = ids[1];

    if (ids.size() == 3) {
        this->failSet = ids[2];

        this->_HasFail = true;
    } else {
        this->_HasFail = false;
    }

    
}
std::vector<unsigned int>* RunConditionInstruction::action(bool async) {

    if (this->evaluationValue->isTruthy()) {
        return this->game->instructionSets[this->successSet].getExecutables();
    } else if (this->_HasFail) {
        return this->game->instructionSets[this->failSet].getExecutables();
    }
    return &Executable::empty;

}
double RunConditionInstruction::getWaitAmount() {
    return 0;
}
