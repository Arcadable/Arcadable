#include "Value.h"
#include "Arduino.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    signed int value,
    bool isPartOfList,
    unsigned short listID
) {
    this->ID = ID;
    this->type = type;
    this->value = value;
    this->isPartOfList = isPartOfList;
    this->listID = listID;
    game = Game::getInstance();
};

int Value::get() {
    switch(type) {
        case integer:
            return value;
        case floatingPoint: {
            _floatingPointUnion.buf[0] = static_cast<char>(value & 0x000000ff);
            _floatingPointUnion.buf[1] = static_cast<char>(( value & 0x0000ff00 ) >> 8);
            _floatingPointUnion.buf[2] = static_cast<char>(( value & 0x00ff0000 ) >> 16);
            _floatingPointUnion.buf[3] = static_cast<char>(( value & 0xff000000 ) >> 24);
            return _floatingPointUnion.number;
        }
        case pixelIndex: {
            int pixelIndex = game->calculations.find(value)->second.result();
            if (game->systemConfig->layoutIsZigZag) {
                int y = pixelIndex / game->systemConfig->screenHeight;
                if (y & 0x01) {
                    int yMul = y * game->systemConfig->screenWidth;
                    pixelIndex = (game->systemConfig->screenWidth - 1) - (pixelIndex - yMul) + yMul;
                }
            }
            return game->pixels[pixelIndex].r + (game->pixels[pixelIndex].g << 8) + (game->pixels[pixelIndex].b << 16);
        }
        case inputPointer: {
            std::map<int, bool>::iterator it = game->systemConfig->inputValues.begin();
            std::advance( it, value );
            return it->second;
        }
        case systemPointer:
            return game->systemConfig->expandedProperties[value];
        case currentTime:
            return millis();
        case list: {
            int result = game->calculations.find(value)->second.result();
            int listID = (( result & 0xff000000 ) >> 16) + (( result & 0x00ff0000 ) >> 16);
            int listIndex = ( result & 0x0000ff00 ) + ( result & 0x000000ff );
            std::pair<std::multimap<int, Value>::iterator, std::multimap<int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<int, Value>::iterator it = values.first;
            std::advance( it, listIndex );
            return it->second.get();
        }
        default:
            return -1;
    };
};

void Value::set(int newValue) {
    switch(type) {
        case integer:
        case floatingPoint:
            value = newValue;
        case pixelIndex: {
            int pixelIndex = game->calculations.find(value)->second.result();
            if (game->systemConfig->layoutIsZigZag) {
                int y = pixelIndex / game->systemConfig->screenHeight;
                if (y & 0x01) {
                    int yMul = y * game->systemConfig->screenWidth;
                    pixelIndex = (game->systemConfig->screenWidth - 1) - (pixelIndex - yMul) + yMul;
                }
            }
            game->pixels[pixelIndex] = newValue;
        }
        default:
            return;
    };
};
