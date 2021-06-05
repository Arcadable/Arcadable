#include "NumberValue.h"


NumberValue::NumberValue(
    unsigned short ID,
    double value,
    unsigned short size
) : Value(ID, ValueType::number) {
    this->value = value;
    this->size = size;
    this->isNumberType = true;
}
NumberValue::NumberValue() {}

void NumberValue::init(std::vector<unsigned short> ids) {}

double NumberValue::getNumber() {
    return value;
}
void NumberValue::setNumber(double newValue) {
    this->value = newValue;
}
bool NumberValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* NumberValue::getValueArray() { return &this->emptyArray; };
void NumberValue::setValueArray(std::vector<unsigned short> newValue) { };