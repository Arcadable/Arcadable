#include "DrawTriangleInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawTriangleInstruction::DrawTriangleInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawTriangle, await) {
    this->display = display;
    this->game = game;

}
DrawTriangleInstruction::DrawTriangleInstruction() {}

void DrawTriangleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->x1Value = this->game->values.find(ids[1])->second;
    this->y1Value = this->game->values.find(ids[2])->second;
    this->x2Value = this->game->values.find(ids[3])->second;
    this->y2Value = this->game->values.find(ids[4])->second; 
    this->x3Value = this->game->values.find(ids[5])->second;
    this->y3Value = this->game->values.find(ids[6])->second;


    
}
std::vector<unsigned int>* DrawTriangleInstruction::action(bool async) {

    int pixel1X = static_cast<int>(this->x1Value->getNumber());
    int pixel1Y = static_cast<int>(this->y1Value->getNumber());
    int pixel2X = static_cast<int>(this->x2Value->getNumber());
    int pixel2Y = static_cast<int>(this->y2Value->getNumber());
    int pixel3X = static_cast<int>(this->x3Value->getNumber());
    int pixel3Y = static_cast<int>(this->y3Value->getNumber());
    CRGB triangleColor = CRGB(this->colorValue->getNumber());
    this->display->canvas.drawTriangle(pixel1X, pixel1Y, pixel2X, pixel2Y, pixel3X, pixel3Y, triangleColor);
    return &Executable::empty;


}
double DrawTriangleInstruction::getWaitAmount() {
    return 0;
}
