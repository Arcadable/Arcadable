#include "ImageValue.h"
#include "../gameState.h"
ImageValue::ImageValue(
    unsigned short ID,
    GameState *game
) : Value(ID, ValueType::image) {
    this->isNumberType = false;
    this->game = game;
}
ImageValue::ImageValue() {}

void ImageValue::init(std::vector<unsigned short> ids) {
    this->data = this->game->values.find(ids[0])->second;
    this->width = this->game->values.find(ids[1])->second;
    this->height = this->game->values.find(ids[2])->second;
    this->keyColor = this->game->values.find(ids[3])->second;
}

bool ImageValue::isTruthy() {
    return this->getNumber() != 0;
}

double ImageValue::getNumber() {return 0;}
void ImageValue::setNumber(double newValue) {}
std::vector<unsigned short>* ImageValue::getValueArray() { return &this->emptyArray; };
void ImageValue::setValueArray(std::vector<unsigned short> newValue) { };