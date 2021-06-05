#include "PixelValue.h"
#include "../gameState.h"
PixelValue::PixelValue(
    unsigned short ID,
    GameState *game,
    DisplayRunner *display
) : Value(ID, ValueType::pixelIndex) {
    this->isNumberType = true;
    this->game = game;
    this->display = display;
}
PixelValue::PixelValue() {}

void PixelValue::init(std::vector<unsigned short> ids) {
    this->XCalc = this->game->values.find(ids[0])->second;
    this->YCalc = this->game->values.find(ids[1])->second;
}
double PixelValue::getNumber() {
    int pixelIndex = static_cast<int>(this->YCalc->getNumber()) * SCREEN_WIDTH + static_cast<int>(this->XCalc->getNumber());
    return (this->display->renderLeds[pixelIndex].r << 16) + (this->display->renderLeds[pixelIndex].g << 8) + this->display->renderLeds[pixelIndex].b;
}
void PixelValue::setNumber(double newValue) {
    int pixelIndex =
        static_cast<int>(this->YCalc->getNumber()) *
        SCREEN_WIDTH +
        static_cast<int>(this->XCalc->getNumber());
    if (pixelIndex > 0 && pixelIndex < SCREEN_WIDTH * SCREEN_HEIGHT) {
        this->display->renderLeds[pixelIndex] = static_cast<int>(newValue);
    }
}
bool PixelValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* PixelValue::getValueArray() { return &this->emptyArray; };
void PixelValue::setValueArray(std::vector<unsigned short> newValue) { };