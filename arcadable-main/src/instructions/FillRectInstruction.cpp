#include "FillRectInstruction.h"
#include "../executable.h"
#include "../gameState.h"

FillRectInstruction::FillRectInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::FillRect, await) {
    this->display = display;
    this->game = game;

}
FillRectInstruction::FillRectInstruction() {}

void FillRectInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->x1Value = this->game->values.find(ids[1])->second;
    this->y1Value = this->game->values.find(ids[2])->second;
    this->x2Value = this->game->values.find(ids[3])->second;
    this->y2Value = this->game->values.find(ids[4])->second; 

    
}
std::vector<unsigned int>* FillRectInstruction::action(bool async) {

    int pixel1X = static_cast<int>(this->x1Value->getNumber());
    int pixel1Y = static_cast<int>(this->y1Value->getNumber());
    int pixel2X = static_cast<int>(this->x2Value->getNumber());
    int pixel2Y = static_cast<int>(this->y2Value->getNumber());
    int w = pixel2X - pixel1X;
    int h = pixel2Y - pixel1Y;
    CRGB drawRectColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.fillRect(pixel1X, pixel1Y, w, h, drawRectColor);
    return &Executable::empty;

}
double FillRectInstruction::getWaitAmount() {
    return 0;
}
