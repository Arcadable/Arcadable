#include "AnalogInputValue.h"

AnalogInputValue::AnalogInputValue(
    unsigned short ID,
    unsigned short index,
    AnalogReader *reader
) : Value(ID, ValueType::analogInputPointer) {
    this->index = index;
    this->isNumberType = true;
    this->reader = reader;
}
AnalogInputValue::AnalogInputValue() {}
void AnalogInputValue::init(std::vector<unsigned short> ids) {}

double AnalogInputValue::getNumber() {
    std::map<unsigned char, unsigned short>::iterator it = this->reader->analogInputValues.begin();
    std::advance( it, this->index );
    return it->second;
}
void AnalogInputValue::setNumber(double newValue) { }
bool AnalogInputValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* AnalogInputValue::getValueArray() { return &this->emptyArray; };
void AnalogInputValue::setValueArray(std::vector<unsigned short> newValue) { };