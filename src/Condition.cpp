#include "Condition.h"
#include "Value.h"
#include "Instruction.h"
#include "Arduino.h"

Condition::Condition(
    unsigned short ID,
    bool rootCondition,
    unsigned short conditionCalculationLeftID,
    unsigned short conditionCalculationRightID,
    RelationalOperator conditionOperator,
    unsigned short conditionSuccessInstructionsID,
    bool hasFailedCondition,
    unsigned short conditionFailedInstructionsID
) {
    this->ID = ID;
    this->rootCondition = rootCondition;
    this->conditionCalculationLeftID = conditionCalculationLeftID;
    this->conditionCalculationRightID = conditionCalculationRightID;
    this->conditionOperator = conditionOperator;
    this->conditionSuccessInstructionsID = conditionSuccessInstructionsID;
    this->hasFailedCondition = hasFailedCondition;
    this->conditionFailedInstructionsID = conditionFailedInstructionsID;
};
    
void Condition::execute(Game *game) const {
    int left = game->calculations.find(conditionCalculationLeftID)->second.result(game);
    int right = game->calculations.find(conditionCalculationRightID)->second.result(game);
    bool validationResult = false;

    switch(conditionOperator) {
        case eq:
            validationResult = left == right;
            break;
        case ne:
            validationResult = left != right;
            break;
        case gt:
            validationResult = left > right;
            break;
        case lt:
            validationResult = left < right;
            break;
        case ge:
            validationResult = left >= right;
            break;
        case le:
            validationResult = left <= right;
            break;
    }
    std::pair<std::multimap<int, Instruction>::iterator, std::multimap<int, Instruction>::iterator> instructions;
    if (validationResult) {
        instructions = game->instructions.equal_range(conditionSuccessInstructionsID);
    } else if (hasFailedCondition) {
        instructions = game->instructions.equal_range(conditionFailedInstructionsID);
    }
    for (std::multimap<int, Instruction>::iterator it = instructions.first; it != instructions.second; it++) {
        it->second.execute(game);
    }
};
