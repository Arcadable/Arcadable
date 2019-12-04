#pragma once
#define Instruction_h
#include "Arcadable.h"

class Instruction {

    public:
        unsigned short ID;
    
    
        /* 
        * If valueLeft.type != gamestatePointer and valueLeft.type != pixel, then valueLeft.value is the ID of another condition to execute
        */
        unsigned short valueLeftID;
    
        /*
        * Ignored when valueLeft.type != gamestatePointer and valueLeft.type != pixel.
        * Otherwise, either the gamestate property, or pixel at index valueLeft.value will be changed to the value/result of calculationRight.
        */
        unsigned short calculationRightID;
    
        Instruction();
        Instruction(
            unsigned short ID,
            unsigned short valueLeftID,
            unsigned short calculationRightID
        );
        void execute(Game *game) const;
};

