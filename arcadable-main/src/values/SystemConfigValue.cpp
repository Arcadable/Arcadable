#include "SystemConfigValue.h"
#include <Arduino.h>
SystemConfigValue::SystemConfigValue(
    unsigned short ID,
    SystemConfigType configType
) : Value(ID, ValueType::systemPointer) {
    this->configType = configType;
    this->isNumberType = true;
}
SystemConfigValue::SystemConfigValue() {}

void SystemConfigValue::init(std::vector<unsigned short> ids) {
    this->startTime = millis();
}
double SystemConfigValue::getNumber() {
    switch(this->configType) {
        case SystemConfigType::screenWidth: {
            return SCREEN_WIDTH;
        }
        case SystemConfigType::screenHeight: {
            return SCREEN_HEIGHT;
        }
        case SystemConfigType::currentMillis: {
            return millis() - this->startTime;
        }
        case SystemConfigType::isZigZag: {
            return LAYOUT_IS_ZIG_ZAG ? (double)1 : (double)0;
        }
        default: return 0;
    }
}
void SystemConfigValue::setNumber(double newValue) { }
bool SystemConfigValue::isTruthy() {
    return this->getNumber() != 0;
}

std::vector<unsigned short>* SystemConfigValue::getValueArray() { return &this->emptyArray; };
void SystemConfigValue::setValueArray(std::vector<unsigned short> newValue) { };