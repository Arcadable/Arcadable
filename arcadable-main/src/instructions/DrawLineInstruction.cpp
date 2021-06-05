#include "DrawLineInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawLineInstruction::DrawLineInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawLine, await) {
    this->display = display;
    this->game = game;

}
DrawLineInstruction::DrawLineInstruction() {}

void DrawLineInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->x1Value = this->game->values.find(ids[1])->second;
    this->y1Value = this->game->values.find(ids[2])->second;
    this->x2Value = this->game->values.find(ids[3])->second;
    this->y2Value = this->game->values.find(ids[4])->second; 

    

}

std::vector<unsigned int>* DrawLineInstruction::action(bool async) {
    int pixel1X = static_cast<int>(this->x1Value->getNumber());
    int pixel1Y = static_cast<int>(this->y1Value->getNumber());
    int pixel2X = static_cast<int>(this->x2Value->getNumber());
    int pixel2Y = static_cast<int>(this->y2Value->getNumber());
    CRGB drawLineColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.drawLine(pixel1X, pixel1Y, pixel2X, pixel2Y, drawLineColor);
    return &Executable::empty;



}
double DrawLineInstruction::getWaitAmount() {
    return 0;
}
