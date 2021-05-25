#include "DrawTextInstruction.h"
#include "../executable.h"
#include "../gameState.h"

DrawTextInstruction::DrawTextInstruction (
    unsigned short ID,
    bool await,
    DisplayRunner *display,
    GameState *game
) : Instruction(ID, InstructionType::DrawText, await) {
    this->display = display;
    this->game = game;

}
DrawTextInstruction::DrawTextInstruction() {}

void DrawTextInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = this->game->values.find(ids[0])->second;
    this->scaleValue = this->game->values.find(ids[1])->second;
    this->textValue = this->game->values.find(ids[2])->second;
    this->xValue = this->game->values.find(ids[3])->second;
    this->yValue = this->game->values.find(ids[4])->second;

    
}
std::vector<unsigned int>* DrawTextInstruction::action(bool async) {

    int scaleValue = static_cast<int>(this->scaleValue->getNumber());
    int xValue = static_cast<int>(this->xValue->getNumber());
    int yValue = static_cast<int>(this->yValue->getNumber());
    std::vector<unsigned short>* textValue = this->textValue->getValueArray();

    CRGB textColor = CRGB(this->colorValue->getNumber());

    std::vector<char> text;
    for ( auto &value : *textValue ) {
        char c = static_cast<int>(this->game->values[value]->getNumber()) & 0x000000FF;
        text.push_back(c);
    }

    unsigned short int extraChars = (SCREEN_WIDTH - (scaleValue * 6 * text.size())) / (scaleValue * 6);
    for (int i = 0; i <= extraChars; i++ ) {
        text.push_back(' ');
    }

    this->display->canvas.setCursor(xValue, yValue);
    this->display->canvas.setTextColor(textColor);
    this->display->canvas.setTextSize(scaleValue);
    this->display->canvas.setTextWrap(false);
    this->display->canvas.print(text.data());
    return &Executable::empty;

}
double DrawTextInstruction::getWaitAmount() {
    return 0;
}
