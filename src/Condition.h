#pragma once
#define Condition_h
#include "ArcadableLib.h"

enum RelationalOperator { eq , ne, gt, lt, ge, le };

class Condition {

    public:
        unsigned short ID;
        Arcadable *game;

        /*
        * If false, this condition will only be executed once it is referenced by another condition.
        */
        bool rootCondition;
    
        unsigned short conditionCalculationLeftID;
        unsigned short conditionCalculationRightID;
        RelationalOperator conditionOperator; 
    
        /* 
        * If the condition validates to true, then the instructions with this ID will be executed
        */
        unsigned short conditionSuccessInstructionsID;

        /*
        * If false, the conditionFailedInstructions will not be executed
        */
        bool hasFailedCondition;
    
        /* 
        * If the condition validates to false, and hasFailedCondition == true, then these instructions with this ID will be executed
        */    
        unsigned short conditionFailedInstructionsID;

        Condition();
        Condition(
            unsigned short ID,
            bool rootCondition,
            unsigned short conditionCalculationLeftID,
            unsigned short conditionCalculationRightID,
            RelationalOperator conditionOperator,
            unsigned short conditionSuccessInstructionsID,
            bool hasFailedCondition,
            unsigned short conditionFailedInstructionsID
        );
        void execute();
};

