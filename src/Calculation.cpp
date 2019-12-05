#include "Calculation.h"
#include "Arduino.h"
Calculation::Calculation(
    unsigned short ID,
    bool ending,
    unsigned short calculationLeftID,
    unsigned short calculationRightID,
    CalculationOperator calculationOperator,
    Game *game
) {
    this->ID = ID;
    this->ending = ending;
  //  this->calculationLeftID = calculationLeftID;
  //  this->calculationRightID = calculationRightID;
    this->calculationOperator = calculationOperator;
    _calculationLeft = game->calculations.find(calculationLeftID)->second;
    _calculationRight = game->calculations.find(calculationRightID)->second;
    _endingValue = game->values.find(calculationLeftID)->second;
};

int Calculation::result(Game *game) {

    if (ending) {
        return _endingValue->get(game);
    }
    int left = _calculationLeft->result(game);
    int right = _calculationRight->result(game);

    switch(calculationOperator) {
        case add:
            return left + right;
        case sub:
            return left - right;
        case mul:
            return left * right;
        case subdiv:
            return left / right;
        case mod:
            return left % right;
        case b_and:
            return left & right;
        case b_or:
            return left | right;
        case b_xor:
            return left ^ right;
        case lsh:
            return left << right;
        case rsh:
            return left >> right;
        default:
            return -1;
    };
};