#pragma once
#define Calculation_h
#include "ArcadableLib.h"
enum CalculationOperator { add, sub, mul, subdiv, mod, b_and, b_or, b_xor, lsh, rsh };

class Calculation {

    public:
        unsigned short ID;
        Arcadable *game;
        bool leftIsValue;
        unsigned short leftID;
        bool rightIsValue;
        unsigned short rightID;
        CalculationOperator calculationOperator;
        bool isStatic;
    
        Calculation() {}
        Calculation(
            unsigned short ID,
            bool leftIsValue,
            unsigned short leftID,
            bool rightIsValue,
            unsigned short rightID,
            CalculationOperator calculationOperator,
            bool isStatic
        );
        int result();
    private:
        int _staticResult;
};
