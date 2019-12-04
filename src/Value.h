#pragma once
#define Value_h
#include "Arcadable.h"

enum ValueType { inputPointer, systemPointer, gamestatePointer, literal, time, pixel };

class Value {
    public:
        unsigned short ID;
        /* 
        * Type determines what the value actually is.
        * If type == inputPointer, then value is the index of the desired input-pin-number in the input-pin-number list (0 is the first input-pin-number in the list)
        * If type == systemPointer, then value is the index of the desired property in the systemConfig. (0 is the first property of systemConfig)
        * If type == gamestatePointer, then value is the index of the desired property in the gamestate. (0 is the first property of gamestate)
        * If type == literal, then the value is just the literal value.
        * If type == time, then the value is the current millis.
        * If type == pixel, then the value is the current RGB color at the given pixel index resulting from the calculation passed as value ID (index = x + y * screenwidth).
        */
        ValueType type;
        signed int value;
        Value(
            unsigned short ID,
            ValueType type,
            signed int value
        );
        
    
        int get(Game * game);
};
