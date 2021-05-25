#include "SetRotationInstruction.h"
#include "../executable.h"
#include "../gameState.h"

SetRotationInstruction::SetRotationInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::SetRotation, await) {
    this->display = display;
    this->game = game;

}
SetRotationInstruction::SetRotationInstruction() {}

void SetRotationInstruction::init(std::vector<unsigned short> ids) {
    this->rotationValue = this->game->values.find(ids[0])->second;

    
}
std::vector<unsigned int>* SetRotationInstruction::action(bool async) {
    this->display->canvas.setRotation(static_cast<int>(this->rotationValue->getNumber()));
    return &Executable::empty;
}
double SetRotationInstruction::getWaitAmount() {
    return 0;
}
