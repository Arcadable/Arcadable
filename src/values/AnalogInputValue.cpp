#include "AnalogInputValue.h"
#include <Arcadable.h>

AnalogInputValue::AnalogInputValue(
    unsigned short ID,
    unsigned short index
) : Value(ID, ValueType::analogInputPointer) {
    this->index = index;
    this->isNumberType = true;
}
AnalogInputValue::AnalogInputValue() {}
void AnalogInputValue::init(std::vector<unsigned short> ids) {}

double AnalogInputValue::getNumber() {
    std::map<unsigned char, unsigned short>::iterator it = Arcadable::getInstance()->systemConfig->analogInputValues.begin();
    std::advance( it, this->index );
    return it->second;
}
void AnalogInputValue::setNumber(double newValue) { }
bool AnalogInputValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* AnalogInputValue::getValueArray() { return &this->emptyArray; };
void AnalogInputValue::setValueArray(std::vector<unsigned short> newValue) { };