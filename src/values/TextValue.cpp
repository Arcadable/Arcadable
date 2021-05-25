#include "TextValue.h"

TextValue::TextValue(
    unsigned short ID,
    unsigned short size
) : Value(ID, ValueType::text) {
    this->isNumberType = false;
    this->size = size;
}
TextValue::TextValue() {}

void TextValue::init(std::vector<unsigned short> ids) {
    this->values = ids;
}

std::vector<unsigned short>* TextValue::getValueArray() {
    return &this->values;
}

void TextValue::setValueArray(std::vector<unsigned short> newValue) {
    this->values = newValue;
}

bool TextValue::isTruthy() {
    return this->size > 0;
}


double TextValue::getNumber() { return 0; };
void TextValue::setNumber(double newValue) { };