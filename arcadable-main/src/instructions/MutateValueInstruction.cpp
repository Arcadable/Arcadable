#include "MutateValueInstruction.h"
#include "../executable.h"
#include "../gameState.h"
MutateValueInstruction::MutateValueInstruction (
    unsigned short ID,
    bool await,
    GameState *game
) : Instruction(ID, InstructionType::MutateValue, await) { 
    this->game = game;
}
MutateValueInstruction::MutateValueInstruction() {}

void MutateValueInstruction::init(std::vector<unsigned short> ids) {
    this->leftValue = this->game->values.find(ids[0])->second;
    this->rightValue = this->game->values.find(ids[1])->second;

    
}

std::vector<unsigned int>* MutateValueInstruction::action(bool async) {
    if(this->leftValue->isNumberType) {
        double right = this->rightValue->getNumber();
        this->leftValue->setNumber(right);
    } else {
        std::vector<unsigned short>* right = this->rightValue->getValueArray();
        this->leftValue->setValueArray(*right);
    }
    return &Executable::empty;


}
double MutateValueInstruction::getWaitAmount() {
    return 0;
}
