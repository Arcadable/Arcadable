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
};

void Instruction::execute(Game *game) const {
    Value valueLeft = game->values.find(valueLeftID)->second;
    if (valueLeft.type == gamestatePointer) {
        game->gamestate[valueLeft.value] = game->calculations.find(calculationRightID)->second.result(game);
    } else if (valueLeft.type == pixel) {
        int pixelIndex = game->calculations.find(valueLeft.value)->second.result(game);
        game->pixels[pixelIndex] = game->calculations.find(calculationRightID)->second.result(game);
    } else {
        game->conditions.find(valueLeft.value)->second.execute(game);
    }
};
