#include "DigitalInputValue.h"

DigitalInputValue::DigitalInputValue(
    unsigned short ID,
    unsigned short index,
    DigitalReader *reader
) : Value(ID, ValueType::digitalInputPointer) {
    this->index = index;
    this->isNumberType = true;
    this->reader = reader;
}
DigitalInputValue::DigitalInputValue() {}

void DigitalInputValue::init(std::vector<unsigned short> ids) {}

double DigitalInputValue::getNumber() {
    std::map<unsigned char, bool>::iterator it = this->reader->digitalInputValues.begin();
    std::advance( it, this->index );
    return it->second;
}
void DigitalInputValue::setNumber(double newValue) { }
bool DigitalInputValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* DigitalInputValue::getValueArray() { return &this->emptyArray; };
void DigitalInputValue::setValueArray(std::vector<unsigned short> newValue) { };