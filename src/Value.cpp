#include "Value.h"
#include "Arduino.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    unsigned int value,
    bool isPartOfList,
    unsigned short listID
) {
    this->ID = ID;
    this->type = type;
    this->value = value;
    this->isPartOfList = isPartOfList;
    this->listID = listID;
    game = Arcadable::getInstance();
};

unsigned int Value::get() {
    switch(type) {
        case integer:
            return value;
        case pixelIndex: {
            unsigned int  pixelIndex = game->calculations.find(value)->second.result();
            if (game->systemConfig->layoutIsZigZag) {
                unsigned int  y = pixelIndex / game->systemConfig->screenWidth;
                if (y & 0x01) {
                    unsigned int  yMul = y * game->systemConfig->screenWidth;
                    pixelIndex = (game->systemConfig->screenWidth - 1) - (pixelIndex - yMul) + yMul;
                }
            }
            return game->pixels[pixelIndex].r + (game->pixels[pixelIndex].g << 8) + (game->pixels[pixelIndex].b << 16);
        }
        case digitalInputPointer: {
            
            std::map<unsigned char, bool>::iterator it = game->systemConfig->digitalInputValues.begin();
            std::advance( it, value );
            return it->second;
        }
        case analogInputPointer: {
            std::map<unsigned char, unsigned short int>::iterator it = game->systemConfig->analogInputValues.begin();
            std::advance( it, value );
            return it->second;
        }
        case systemPointer:
            return game->systemConfig->expandedProperties[value];
        case currentTime:
            return millis();
        case list: {
            unsigned short int listID = ( value & 0xffff0000 ) >> 16;
            unsigned short int listPosCalcID = value & 0x0000ffff;
            unsigned int listPos = game->calculations.find(listPosCalcID)->second.result();
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<unsigned short int, Value>::iterator it = values.first;
            std::advance( it, listPos );
            return it->second.get();
        }
        default:
            return -1;
    };
};

void Value::set(unsigned int newValue) {
    switch(type) {
        case integer:
            value = newValue;
            break;
        case pixelIndex: {
            unsigned int pixelIndex = game->calculations.find(value)->second.result();
            if (game->systemConfig->layoutIsZigZag) {
                unsigned int y = pixelIndex / game->systemConfig->screenWidth;
                if (y & 0x01) {
                    unsigned int yMul = y * game->systemConfig->screenWidth;
                    pixelIndex = (game->systemConfig->screenWidth - 1) - (pixelIndex - yMul) + yMul;
                }
            }
            if (pixelIndex > 0 && pixelIndex < game->systemConfig->screenWidth * game->systemConfig->screenHeight) {
                game->pixels[pixelIndex] = newValue;
            }
            break;
        }
        case list: {
            unsigned short int listID = ( value & 0xffff0000 ) >> 16;
            unsigned short int listPosCalcID = value & 0x0000ffff;
            unsigned int listPos = game->calculations.find(listPosCalcID)->second.result();
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<unsigned short int, Value>::iterator it = values.first;
            std::advance( it, listPos );
            Value *listValue = &it->second;

            if (listValue->type == integer || listValue->type == pixelIndex) {
                listValue->set(newValue);
            }
            break;
        }
        default:
            return;
    };
};
