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
    this->_staticResult = (double)(-1);
    game = Arcadable::getInstance();
};

double Calculation::result() {

    if (isStatic && _staticResult != (double)(-1)) {
        return _staticResult;
    }

    double left = leftIsValue ? game->values.find(leftID)->second.get() : game->calculations.find(leftID)->second.result();
    double right = rightIsValue ? game->values.find(rightID)->second.get() : game->calculations.find(rightID)->second.result();

    double result = -1;
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
            result =  static_cast<int>(left) % static_cast<int>(right);
            break;
        case b_and:
            result =  static_cast<int>(left) & static_cast<int>(right);
            break;
        case b_or:
            result =  static_cast<int>(left) | static_cast<int>(right);
            break;
        case b_xor:
            result =  static_cast<int>(left) ^ static_cast<int>(right);
            break;
        case lsh:
            result =  static_cast<int>(left) << static_cast<int>(right);
            break;
        case rsh:
            result =  static_cast<int>(left) >> static_cast<int>(right);
            break;
        case math_pow:
            result =  pow(left, right);
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