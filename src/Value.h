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
    list,
    text
};

class Value {
    public:
        unsigned short ID;
        Arcadable *game;
        ValueType type;
        int value;
        bool isPartOfList;
        unsigned short listID;
        Value();
        Value(
            unsigned short ID,
            ValueType type,
            int value,
            bool isPartOfList,
            unsigned short listID
        );
        
        float get();
        void set(int newValue);
};
