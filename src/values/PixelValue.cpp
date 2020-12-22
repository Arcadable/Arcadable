#include "PixelValue.h"
#include "Arcadable.h"

PixelValue::PixelValue(
    unsigned short ID,
    bool await
) : Value(ID, ValueType::pixelIndex) {
    this->isNumberType = true;
}
PixelValue::PixelValue() {}

void PixelValue::init(std::vector<unsigned short> ids) {
    this->XCalc = Arcadable::getInstance()->values.find(ids[0])->second;
    this->YCalc = Arcadable::getInstance()->values.find(ids[1])->second;
}
double PixelValue::getNumber() {
    int pixelIndex =
        static_cast<int>(this->YCalc->getNumber()) *
        Arcadable::getInstance()->systemConfig->screenWidth +
        static_cast<int>(this->XCalc->getNumber());
    return (Arcadable::getInstance()->pixelsBuffer[pixelIndex].r << 16) + (Arcadable::getInstance()->pixelsBuffer[pixelIndex].g << 8) + Arcadable::getInstance()->pixelsBuffer[pixelIndex].b;
}
void PixelValue::setNumber(double newValue) {
    int pixelIndex =
        static_cast<int>(this->YCalc->getNumber()) *
        Arcadable::getInstance()->systemConfig->screenWidth +
        static_cast<int>(this->XCalc->getNumber());
    if (pixelIndex > 0 && pixelIndex < Arcadable::getInstance()->systemConfig->screenWidth * Arcadable::getInstance()->systemConfig->screenHeight) {
        Arcadable::getInstance()->pixels[pixelIndex] = static_cast<int>(newValue);
    }
}
bool PixelValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* PixelValue::getValueArray() { return &this->emptyArray; };
void PixelValue::setValueArray(std::vector<unsigned short> newValue) { };