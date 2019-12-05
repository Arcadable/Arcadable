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
    unsigned short conditionFailedInstructionsID,
    Game *game
) {
    this->ID = ID;
    this->rootCondition = rootCondition;
    this->conditionCalculationLeftID = conditionCalculationLeftID;
    this->conditionCalculationRightID = conditionCalculationRightID;
    this->conditionOperator = conditionOperator;
    this->hasFailedCondition = hasFailedCondition;

    std::pair<std::multimap<int, Instruction>::iterator, std::multimap<int, Instruction>::iterator> instructions;

    instructions = game->instructions.equal_range(conditionSuccessInstructionsID);
    for (std::multimap<int, Instruction>::iterator it = instructions.first; it != instructions.second; it++) {
        _successInstructions.push_back(&it->second);
    }

    instructions = game->instructions.equal_range(conditionFailedInstructionsID);
    for (std::multimap<int, Instruction>::iterator it = instructions.first; it != instructions.second; it++) {
        _failedInstructions.push_back(&it->second);
    }
    _conditionCalculationLeft = game->calculations.find(conditionCalculationLeftID)->second;
    _conditionCalculationRight = game->calculations.find(conditionCalculationRightID)->second;
};
    
void Condition::execute(Game *game) const {
    int left = _conditionCalculationLeft->result(game);
    int right = _conditionCalculationRight->result(game);
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
    std::vector<Instruction> instructions;
    if (validationResult) {
        for ( Instruction *instruction : _successInstructions ) {
            instruction->execute(game);
        }
    } else if (hasFailedCondition) {
        for ( Instruction *instruction : _failedInstructions ) {
            instruction->execute(game);
        }
    }
};
