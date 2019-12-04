#include "Value.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    signed int value
) {
    this->ID = ID;
    this->type = type;
    this->value = value;
};

int Value::get(Game * game) {

    switch(type) {
        case inputPointer:
            return game->inputs->inputValues[value];
        case systemPointer:
            return game->systemConfig->expandedProperties[value];
        case gamestatePointer:
            return game->gamestate.find(value)->second;
        case literal:
            return value;
        case time:
            return millis();
        case pixel: {
                int pixelIndex = game->calculations.find(value)->second.result(game);
                return game->pixels[pixelIndex].r + (game->pixels[pixelIndex].g << 8) + (game->pixels[pixelIndex].b << 16);
            }
        default:
            return -1;
    };
};
