#include "WaitInstruction.h"
#include "Arduino.h"
#include "../executable.h"
#include "../gameState.h"
WaitInstruction::WaitInstruction (
    unsigned short ID,
    bool await,
    GameState *game
) : Instruction(ID, InstructionType::Wait, await) {
    this->game = game;
}
WaitInstruction::WaitInstruction() {}

void WaitInstruction::init(std::vector<unsigned short> ids) {
    this->amountValue = this->game->values.find(ids[0])->second;

    
}

std::vector<unsigned int>* WaitInstruction::action(bool async) {
    return &Executable::empty;
}
double WaitInstruction::getWaitAmount() {
    return this->amountValue->getNumber();
}
