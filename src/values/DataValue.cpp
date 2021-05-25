#include "DataValue.h"

DataValue::DataValue (
    unsigned short ID,
    unsigned short size
) : Value(ID, ValueType::data) {
    this->isNumberType = false;
    this->size = size;
    this->values = std::vector<short unsigned int>(this->size);
};
DataValue::DataValue() {}

void DataValue::init(std::vector<short unsigned int> ids) {
    this->values = ids;
}


std::vector<short unsigned int>* DataValue::getValueArray() {
    return &this->values;
}

void DataValue::setValueArray(std::vector<short unsigned int> newValue) {
    this->values = newValue;
}

bool DataValue::isTruthy() {
    return this->size > 0;
}

double DataValue::getNumber() { return 0; };
void DataValue::setNumber(double newValue) { };