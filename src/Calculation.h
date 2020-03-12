#pragma once
#define Calculation_h
#include "ArcadableLib.h"
#include <cmath>
enum CalculationOperator { add, sub, mul, subdiv, mod, b_and, b_or, b_xor, lsh, rsh, math_pow };

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
        double result();
    private:
        double _staticResult;
};
