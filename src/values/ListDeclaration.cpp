#include "ListDeclaration.h"
#include "Arcadable.h"

ListDeclaration::ListDeclaration (
    unsigned short ID,
    unsigned short size
) : Value(ID, ValueType::listDeclaration) {
    this->isNumberType = false;
    this->size = size;
    this->values = std::vector<unsigned short>(this->size);
};
ListDeclaration::ListDeclaration() {}

void ListDeclaration::init(std::vector<unsigned short> ids) {
    this->values = ids;
}


std::vector<unsigned short>* ListDeclaration::getValueArray() {
    return &this->values;
}

void ListDeclaration::setValueArray(std::vector<unsigned short> newValue) {
    this->values = newValue;
}

bool ListDeclaration::isTruthy() {
    return this->size > 0;
}

double ListDeclaration::getNumber() { return 0; };
void ListDeclaration::setNumber(double newValue) { };