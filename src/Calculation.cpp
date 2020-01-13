#include "Calculation.h"
#include "Arduino.h"
Calculation::Calculation(
    unsigned short ID,
    bool leftIsValue,
    unsigned short leftID,
    bool rightIsValue,
    unsigned short rightID,
    CalculationOperator calculationOperator,
    bool isStatic
) {
    this->ID = ID;
    this->calculationOperator = calculationOperator;
    this->leftIsValue = leftIsValue;
    this->leftID = leftID;
    this->rightIsValue = rightIsValue;
    this->rightID = rightID;
    this->isStatic = isStatic;
    this->_staticResult = (unsigned int)(-1);
    game = Arcadable::getInstance();
};

unsigned int Calculation::result() {

    if (isStatic && _staticResult != (unsigned int)(-1)) {
        return _staticResult;
    }

    unsigned int left = leftIsValue ? game->values.find(leftID)->second.get() : game->calculations.find(leftID)->second.result();
    unsigned int right = rightIsValue ? game->values.find(rightID)->second.get() : game->calculations.find(rightID)->second.result();

    unsigned int result = -1;
    switch(calculationOperator) {
        case add:
            result = left + right;
            break;
        case sub:
            result =  left - right;
            break;
        case mul:
            result =  left * right;
            break;
        case subdiv:
            result =  left / right;
            break;
        case mod:
            result =  left % right;
            break;
        case b_and:
            result =  left & right;
            break;
        case b_or:
            result =  left | right;
            break;
        case b_xor:
            result =  left ^ right;
            break;
        case lsh:
            result =  left << right;
            break;
        case rsh:
            result =  left >> right;
            break;
        default:
            result =  -1;
            break;
    };
    if(isStatic) {
        _staticResult = result;
    }
    return result;
    
};