#pragma once
#define Value_h
#include "ArcadableLib.h"

enum ValueType {

    number,
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
        double value;
        bool isPartOfList;
        unsigned short listID;
        Value();
        Value(
            unsigned short ID,
            ValueType type,
            double value,
            bool isPartOfList,
            unsigned short listID
        );
        
        double get();
        void set(double newValue);
};
