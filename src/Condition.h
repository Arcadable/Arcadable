#pragma once
#define Condition_h
#include "ArcadableLib.h"

enum RelationalOperator { eq , ne, gt, lt, ge, le };

class Condition {

    public:
        unsigned short ID;
        Arcadable *game;
        bool rootCondition;
        bool leftIsValue;
        unsigned short leftID;
        bool rightIsValue;
        unsigned short rightID;
        RelationalOperator conditionOperator; 
        unsigned short conditionSuccessInstructionsID;
        bool hasFailedCondition;  
        unsigned short conditionFailedInstructionsID;

        Condition();
        Condition(
            unsigned short ID,
            bool rootCondition,
            bool leftIsValue,
            unsigned short leftID,
            bool rightIsValue,
            unsigned short rightID,
            RelationalOperator conditionOperator,
            unsigned short conditionSuccessInstructionsID,
            bool hasFailedCondition,
            unsigned short conditionFailedInstructionsID
        );
        void execute();
};

