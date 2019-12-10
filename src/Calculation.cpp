#include "Calculation.h"
#include "Arduino.h"
Calculation::Calculation(
    unsigned short ID,
    bool ending,
    unsigned short calculationLeftID,
    unsigned short calculationRightID,
    CalculationOperator calculationOperator
) {
    this->ID = ID;
    this->ending = ending;
    this->calculationOperator = calculationOperator;
    this->calculationLeftID = calculationLeftID;
    this->calculationRightID = calculationRightID;
    game = Arcadable::getInstance();
};

int Calculation::result() {
    if (ending) {
        return game->values.find(calculationLeftID)->second.get();
    }
    int left = game->calculations.find(calculationLeftID)->second.result();
    int right = game->calculations.find(calculationRightID)->second.result();
    
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