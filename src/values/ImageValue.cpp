#include "ImageValue.h"
#include "Arcadable.h"

ImageValue::ImageValue(
    unsigned short ID,
    bool await
) : Value(ID, ValueType::image) {
    this->isNumberType = false;
}
ImageValue::ImageValue() {}

void ImageValue::init(std::vector<unsigned short> ids) {
    this->data = Arcadable::getInstance()->values.find(ids[0])->second;
    this->width = Arcadable::getInstance()->values.find(ids[1])->second;
    this->height = Arcadable::getInstance()->values.find(ids[2])->second;
    this->keyColor = Arcadable::getInstance()->values.find(ids[3])->second;
}

bool ImageValue::isTruthy() {
    return this->getNumber() != 0;
}

double ImageValue::getNumber() {return 0;}
void ImageValue::setNumber(double newValue) {}
std::vector<unsigned short>* ImageValue::getValueArray() { return &this->emptyArray; };
void ImageValue::setValueArray(std::vector<unsigned short> newValue) { };