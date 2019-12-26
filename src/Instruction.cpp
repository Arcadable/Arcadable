#include "Condition.h"
#include "Value.h"
#include "Arduino.h"

Instruction::Instruction(
    unsigned short ID,
    unsigned short leftID,
    bool rightIsValue,
    unsigned short rightID,
    InstructionType instructionType
) {
    this->ID = ID;
    this->leftID = leftID;
    this->rightIsValue = rightIsValue;
    this->rightID = rightID;
    this->instructionType = instructionType;
    game = Arcadable::getInstance();
};

void Instruction::execute() {
    if (instructionType == MutateValue) {
        Value *valueLeft = &game->values.find(leftID)->second;

        if (
            valueLeft->type == integer ||
            valueLeft->type == floatingPoint ||
            valueLeft->type == pixelIndex
        ) {
            int right = rightIsValue ? game->values.find(rightID)->second.get() : game->calculations.find(rightID)->second.result();
            valueLeft->set(right);
        }
    } else {
        game->conditions.find(leftID)->second.execute();
    }
};
