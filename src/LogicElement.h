#pragma once
#define LogicElement_h
#include "Arcadable.h"

class LogicElement {
    public:
        unsigned short int ID;
        Arcadable *game;
        LogicElement();
        LogicElement(
            unsigned short ID
        );

};
