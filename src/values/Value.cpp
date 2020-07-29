#include "Value.h"
#include "Arduino.h"
Value::Value (
    unsigned short ID,
    ValueType type
) {
    this->type = type;
    this->ID = ID;
}
Value::Value () { }
double Value::getNumber() { return 0; }
void Value::setNumber(double newValue) {}
std::vector<unsigned short>* Value::getValueArray() {
    return &this->emptyArray;
}
void Value::setValueArray(std::vector<unsigned short> newValue) {}
bool Value::isTruthy() {
    return false;
}
void Value::init(std::vector<unsigned short> ids) {}