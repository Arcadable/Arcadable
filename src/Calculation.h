#pragma once
#define Calculation_h
#include "ArcadableLib.h"
enum CalculationOperator { add, sub, mul, subdiv, mod, b_and, b_or, b_xor, lsh, rsh };

class Calculation {

    public:
        unsigned short ID;
        Game *game;
        /*
        * To make nested calculations possible, a calculation references other calculations to perform arithmetics on.
        * If ending == true, the loop of calculations is broken, calculationRight is ignored and calculationLeftID is assumed to be a valueID, which is then returned without performing any arithmetics.
        */
        bool ending;
        unsigned short calculationLeftID;
        unsigned short calculationRightID;
        CalculationOperator calculationOperator;
    
        Calculation() {}
        Calculation(
            unsigned short ID,
            bool ending,
            unsigned short calculationLeftID,
            unsigned short calculationRightID,
            CalculationOperator calculationOperator
        );
        int result();
};
