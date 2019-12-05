#pragma once
#define Instruction_h
#include "Arcadable.h"

class Instruction {

    public:
        unsigned short ID;
    
    
        /* 
        * If valueLeft.type is not a mutable valuetype, then valueLeft.value is the ID of another condition to execute
        */
        //unsigned short valueLeftID;
    
        /*
        * Ignored when valueLeft.type is not a mutable valuetype.
        * Otherwise, the mutable valuetype will be changed to the value/result of calculationRight.
        */
        //unsigned short calculationRight;
    
        Instruction();
        Instruction(
            unsigned short ID,
            unsigned short valueLeftID,
            unsigned short calculationRightID,
            Game *game
        );
        void execute(Game *game) const;
    private:
        Calculation *_calculationRight;
        Value *_valueLeft;
        Calculation *_pixelCalculation;
};

