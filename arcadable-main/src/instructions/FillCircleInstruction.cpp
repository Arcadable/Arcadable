#include "FillCircleInstruction.h"
#include "../executable.h"
#include "../gameState.h"

FillCircleInstruction::FillCircleInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::FillCircle, await) {
    this->display = display;
    this->game = game;

}
FillCircleInstruction::FillCircleInstruction() {}

void FillCircleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->radiusValue = this->game->values.find(ids[1])->second;
    this->xValue = this->game->values.find(ids[2])->second;
    this->yValue = this->game->values.find(ids[3])->second;

    
}

std::vector<unsigned int>* FillCircleInstruction::action(bool async) {


    int radiusValue = static_cast<int>(this->radiusValue->getNumber());
    int xValue = static_cast<int>(this->xValue->getNumber());
    int yValue = static_cast<int>(this->yValue->getNumber());

    CRGB drawCircleColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.fillCircle(xValue, yValue, radiusValue, drawCircleColor);
    return &Executable::empty;


}
double FillCircleInstruction::getWaitAmount() {
    return 0;
}
