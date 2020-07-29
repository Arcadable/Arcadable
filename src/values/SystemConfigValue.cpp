#include "SystemConfigValue.h"
#include "Arcadable.h"

SystemConfigValue::SystemConfigValue(
    unsigned short ID,
    SystemConfigType configType
) : Value(ID, ValueType::systemPointer) {
    this->configType = configType;
    this->isNumberType = true;
}
SystemConfigValue::SystemConfigValue() {}

void SystemConfigValue::init(std::vector<unsigned short> ids) {}
double SystemConfigValue::getNumber() {
    return Arcadable::getInstance()->systemConfig->get(this->configType);
}
void SystemConfigValue::setNumber(double newValue) { }
bool SystemConfigValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* SystemConfigValue::getValueArray() { return &this->emptyArray; };
void SystemConfigValue::setValueArray(std::vector<unsigned short> newValue) { };