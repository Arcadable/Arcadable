#include "DrawCircleInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawCircleInstruction::DrawCircleInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawCircle, await) {
    this->display = display;
    this->game = game;

}
DrawCircleInstruction::DrawCircleInstruction() {}

void DrawCircleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->radiusValue = this->game->values.find(ids[1])->second;
    this->xValue = this->game->values.find(ids[2])->second;
    this->yValue = this->game->values.find(ids[3])->second;

    
}
std::vector<unsigned int>* DrawCircleInstruction::action(bool async) {
    int radiusValue = static_cast<int>(this->radiusValue->getNumber());
    int xValue = static_cast<int>(this->xValue->getNumber());
    int yValue = static_cast<int>(this->yValue->getNumber());

    CRGB drawCircleColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.drawCircle(xValue, yValue, radiusValue, drawCircleColor);
    return &Executable::empty;


}
double DrawCircleInstruction::getWaitAmount() {
    return 0;
}
