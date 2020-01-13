#pragma once
#define Value_h
#include "ArcadableLib.h"

enum ValueType {

    integer,
    pixelIndex,
    digitalInputPointer,
    analogInputPointer,
    systemPointer,
    currentTime,
    list
};

class Value {
    public:
        unsigned short ID;
        Arcadable *game;
        ValueType type;
        unsigned int value;
        bool isPartOfList;
        unsigned short listID;
        Value();
        Value(
            unsigned short ID,
            ValueType type,
            unsigned int value,
            bool isPartOfList,
            unsigned short listID
        );
        
        unsigned int get();
        void set(unsigned int newValue);
};
