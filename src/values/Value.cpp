#include "Value.h"


Value::Value (
    unsigned short ID,
    ValueType type
) : LogicElement(ID) {
    this->type = type;
}