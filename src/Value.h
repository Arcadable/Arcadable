#pragma once
#define Value_h
#include "ArcadableLib.h"

enum ValueType {
    /* 
    * Type determines what the value actually is.
    * If type == inputPointer, then the value is then value is the index of the desired input-pin-number in the input-pin-number list (0 is the first input-pin-number in the list)
    * If type == systemPointer, then value is the index of the desired property in the systemConfig. (0 is the first property of systemConfig)
    * If type == currentTime, then the value is the current millis.
    * If type == list, then the value is a calculation ID. The first 2 bytes of the calculation result is the list ID. The last 2 bytes is the list index of which to return the value from.
    * If type == integer, then the value is just the literal integer value.
    * If type == float, then the value is just the literal float value.
    * If type == pixelIndex, then the value is the current RGB color at the given pixel index resulting from the calculation passed as value ID (index = x + y * screenwidth).
    */
    // Mutable types
    integer,
    floatingPoint,
    pixelIndex,

    // Static types
    inputPointer,
    systemPointer,
    currentTime,
    list
};

class Value {
    public:
        unsigned short ID;
        Game *game;
        ValueType type;
        signed int value;
        bool isPartOfList;
        unsigned short listID;
        Value();
        Value(
            unsigned short ID,
            ValueType type,
            signed int value,
            bool isPartOfList,
            unsigned short listID
        );
        
        int get();
        void set(int newValue);
    private:
        union floatingPointUnion {
            unsigned char buf[4];
            float number;
        }_floatingPointUnion;
};
