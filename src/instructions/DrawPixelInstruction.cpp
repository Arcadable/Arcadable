#include "DrawPixelInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawPixelInstruction::DrawPixelInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawPixel, await) {
    this->display = display;
    this->game = game;

}
DrawPixelInstruction::DrawPixelInstruction() {}

void DrawPixelInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->xValue = this->game->values.find(ids[1])->second;
    this->yValue = this->game->values.find(ids[2])->second;

    

}

std::vector<unsigned int>* DrawPixelInstruction::action(bool async) {

    int pixelX = static_cast<int>(this->xValue->getNumber());
    int pixelY = static_cast<int>(this->yValue->getNumber());

    CRGB pixelColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.drawPixel(pixelX, pixelY, pixelColor);
    return &Executable::empty;




}
double DrawPixelInstruction::getWaitAmount() {
    return 0;
}
