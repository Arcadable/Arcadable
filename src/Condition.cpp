#include "Condition.h"
#include "Value.h"
#include "Instruction.h"
#include "Arduino.h"

Condition::Condition(
    unsigned short ID,
    bool rootCondition,
    bool leftIsValue,
    unsigned short leftID,
    bool rightIsValue,
    unsigned short rightID,
    RelationalOperator conditionOperator,
    unsigned short conditionSuccessInstructionsID,
    bool hasFailedCondition,
    unsigned short conditionFailedInstructionsID
) {
    this->ID = ID;
    this->rootCondition = rootCondition;
    this->leftIsValue = leftIsValue;
    this->leftID = leftID;
    this->rightIsValue = rightIsValue;
    this->rightID = rightID;
    this->conditionOperator = conditionOperator;
    this->hasFailedCondition = hasFailedCondition;
    this->conditionSuccessInstructionsID = conditionSuccessInstructionsID;
    this->conditionFailedInstructionsID = conditionFailedInstructionsID;
    game = Arcadable::getInstance();
};
    
void Condition::execute() {
    unsigned int left = leftIsValue ? game->values.find(leftID)->second.get() : game->calculations.find(leftID)->second.result();
    unsigned int right = rightIsValue ? game->values.find(rightID)->second.get() : game->calculations.find(rightID)->second.result();

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

    std::pair<std::multimap<unsigned short int, Instruction>::iterator, std::multimap<unsigned short int, Instruction>::iterator> instructions;
    if (validationResult) {
        instructions = game->instructions.equal_range(conditionSuccessInstructionsID);
    } else if (hasFailedCondition) {
        instructions = game->instructions.equal_range(conditionFailedInstructionsID);
    }
    
    for (std::multimap<unsigned short int, Instruction>::iterator it = instructions.first; it != instructions.second; it++) {
        it->second.execute();
    }


};
