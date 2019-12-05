#include "Value.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    signed int value,
    bool isPartOfList,
    unsigned short listID,
    Game * game
) {
    this->ID = ID;
    this->type = type;
    this->value = value;
    this->isPartOfList = isPartOfList;
    this->listID = listID;

    if (type == pixelIndex || type == list) {
        _calculation = &game->calculations.find(value)->second;
    }
};
int Value::get(Game * game) {
    switch(type) {
        case integer:
            return value;
        case floatingPoint: {
                byte bytes[] {
                    value & 0x000000ff,
                    ( value & 0x0000ff00 ) >> 8,
                    ( value & 0x00ff0000 ) >> 16,
                    ( value & 0xff000000 ) >> 24
                };
                return *(float*)(bytes);
            }
        case pixelIndex: {
            int pixelIndex = _calculation->result(game);
            return game->pixels[pixelIndex].r + (game->pixels[pixelIndex].g << 8) + (game->pixels[pixelIndex].b << 16);
        }
        case systemConfig:
            return game->systemConfig->expandedProperties[value];
        case currentTime:
            return millis();
        case list:
            int result = _calculation->result(game);
            int listID = (( result & 0xff000000 ) >> 16) + (( result & 0x00ff0000 ) >> 16);
            int listIndex = ( result & 0x0000ff00 ) + ( result & 0x000000ff );
            std::pair<std::multimap<int, Value>::iterator, std::multimap<int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<int, Value>::iterator it = values.first;
            std::advance( it, listIndex );
            return it->second.get(game);
        case input:
            game->systemConfig->inputValues[value];
        default:
            return -1;
    };
};
