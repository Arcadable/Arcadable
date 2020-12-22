#include "ListValue.h"
#include "Arcadable.h"
#include "values/ListDeclaration.h"
ListValue::ListValue (
    unsigned short ID,
    bool await
) : Value(ID, ValueType::listValue) {
    this->isNumberType = false;
}
ListValue::ListValue() {
    this->isNumberType = false;
}
void ListValue::init(std::vector<unsigned short> ids) {
    this->listValue = static_cast<ListDeclaration*>(Arcadable::getInstance()->values.find(ids[0])->second);
    this->listIndex = Arcadable::getInstance()->values.find(ids[1])->second;
    this->isNumberType = Arcadable::getInstance()->values[this->listValue->getValueArray()->at(0)]->isNumberType;

}

double ListValue::getNumber() {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        return Arcadable::getInstance()->values[this->listValue->getValueArray()->at(index)]->getNumber();
    } else {
        return -1;
    }
}

std::vector<unsigned short>* ListValue::getValueArray() {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        return Arcadable::getInstance()->values[this->listValue->getValueArray()->at(index)]->getValueArray();
    } else {
        return &this->emptyArray;
    }
}
void ListValue::setNumber(double newValue) {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        Arcadable::getInstance()->values[this->listValue->getValueArray()->at(index)]->setNumber(newValue);
    }
}
void ListValue::setValueArray(std::vector<unsigned short> newValue) {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        Arcadable::getInstance()->values[this->listValue->getValueArray()->at(index)]->setValueArray(newValue);
    }
}
bool ListValue::isTruthy() {
    double index = this->listIndex->getNumber();
    return Arcadable::getInstance()->values[this->listValue->getValueArray()->at(index)]->isTruthy();
}
