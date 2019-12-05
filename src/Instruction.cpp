#include "Condition.h"
#include "Value.h"
#include "Arduino.h"

Instruction::Instruction(
    unsigned short ID,
    unsigned short valueLeftID,
    unsigned short calculationRightID
) {
    this->ID = ID;
    _calculationRight = &game->calculations.find(calculationRightID)->second;
    _valueLeft = &game->values.find(valueLeftID)->second;
    if (_valueLeft->type == pixelIndex) {
        _pixelCalculation = &game->calculations.find(_valueLeft->value)->second;
    }
};

void Instruction::execute(Game *game) const {
    if (_valueLeft->type == integer || _valueLeft->type == floatingPoint) {
        game->values[_valueLeft->ID] = _calculationRight->result(game);
    } else if (_valueLeft->type == pixelIndex) {
        int pixelIndex = _pixelCalculation->result(game);
        game->pixels[pixelIndex] = _calculationRight->result(game);
    } else {
        game->conditions.find(valueLeft.value)->second.execute(game);
    }
};
