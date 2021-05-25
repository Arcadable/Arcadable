#include "DrawImageInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawImageInstruction::DrawImageInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawRect, await) {
    this->display = display;
    this->game = game;

}
DrawImageInstruction::DrawImageInstruction() {}

void DrawImageInstruction::init(std::vector<unsigned short> ids) {
    this->xValue = this->game->values.find(ids[0])->second;
    this->yValue = this->game->values.find(ids[1])->second;
    this->imageValue = this->game->values.find(ids[2])->second;

    

}

std::vector<unsigned int>* DrawImageInstruction::action(bool async) {
    std::vector<short unsigned int>* data = static_cast<ImageValue*>(this->imageValue)->data->getValueArray();
    int x = static_cast<int>(this->xValue->getNumber());
    int y = static_cast<int>(this->yValue->getNumber());
    int w = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->width->getNumber());
    int h = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->height->getNumber());
    int k = static_cast<int>(static_cast<ImageValue*>(this->imageValue)->keyColor->getNumber());

    this->display->canvas.drawImage(x, y, w, h, k, data);
    return &Executable::empty;



}
double DrawImageInstruction::getWaitAmount() {
    return 0;
}
