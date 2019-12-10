#include "Condition.h"
#include "Value.h"
#include "Arduino.h"

Instruction::Instruction(
    unsigned short ID,
    unsigned short valueLeftID,
    unsigned short calculationRightID
) {
    this->ID = ID;
    this->valueLeftID = valueLeftID;
    this->calculationRightID = calculationRightID;
    game = Arcadable::getInstance();
};

void Instruction::execute() {
    Value *value = &game->values.find(valueLeftID)->second;
    if (value->type == integer || value->type == floatingPoint || value->type == pixelIndex) {
        value->set(game->calculations.find(calculationRightID)->second.result());
    } else {
        game->conditions.find(value->value)->second.execute();
    }
};
