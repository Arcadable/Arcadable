#include "ListValue.h"
#include "ListDeclaration.h"
#include "../gameState.h"
ListValue::ListValue (
    unsigned short ID,
    GameState *game
) : Value(ID, ValueType::listValue) {
    this->isNumberType = false;
    this->game = game;
}
ListValue::ListValue() {
    this->isNumberType = false;
}
void ListValue::init(std::vector<unsigned short> ids) {
    this->listValue = static_cast<ListDeclaration*>(this->game->values.find(ids[0])->second);
    this->listIndex = this->game->values.find(ids[1])->second;
    this->isNumberType = this->game->values[this->listValue->getValueArray()->at(0)]->isNumberType;

}

double ListValue::getNumber() {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        return this->game->values[this->listValue->getValueArray()->at(index)]->getNumber();
    } else {
        return -1;
    }
}

std::vector<unsigned short>* ListValue::getValueArray() {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        return this->game->values[this->listValue->getValueArray()->at(index)]->getValueArray();
    } else {
        return &this->emptyArray;
    }
}
void ListValue::setNumber(double newValue) {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        this->game->values[this->listValue->getValueArray()->at(index)]->setNumber(newValue);
    }
}
void ListValue::setValueArray(std::vector<unsigned short> newValue) {
    double index = this->listIndex->getNumber();
    if (index >= 0 && index < this->listValue->size) {
        this->game->values[this->listValue->getValueArray()->at(index)]->setValueArray(newValue);
    }
}
bool ListValue::isTruthy() {
    double index = this->listIndex->getNumber();
    return this->game->values[this->listValue->getValueArray()->at(index)]->isTruthy();
}
