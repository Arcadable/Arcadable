#include "RunSetInstruction.h"
#include "../executable.h"
#include "../gameState.h"

RunSetInstruction::RunSetInstruction (
    unsigned short ID,
    bool await,
    GameState *game
) : Instruction(ID, InstructionType::RunSet, await) {
    this->game = game;

}
RunSetInstruction::RunSetInstruction() {}

void RunSetInstruction::init(std::vector<unsigned short> ids) {
    this->set = ids[0];

    
}
std::vector<unsigned int>* RunSetInstruction::action(bool async) {
    return this->game->instructionSets[this->set].getExecutables();

}
double RunSetInstruction::getWaitAmount() {
    return 0;
}
