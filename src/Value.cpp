#include "Value.h"
#include "Arduino.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    int value,
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

int Value::get() {
    switch(type) {
        case integer:
        case text:
            return value;
        case pixelIndex: {
            unsigned short int xCalc = ( value & 0xffff0000 ) >> 16;
            unsigned short int yCalc = value & 0x0000ffff;
            int pixelIndex =
                game->calculations.find(yCalc)->second.result() *
                game->systemConfig->screenWidth +
                game->calculations.find(xCalc)->second.result();
            return  (game->pixels[pixelIndex].r << 16) + (game->pixels[pixelIndex].g << 8) + game->pixels[pixelIndex].b;
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
            int listPos = game->calculations.find(listPosCalcID)->second.result();
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<unsigned short int, Value>::iterator it = values.first;
            std::advance( it, listPos );
            return game->values.find(it->second.ID)->second.get();
        }
        default:
            return -1;
    };
};

void Value::set(int newValue) {
    switch(type) {
        case integer:
        case text:
            value = newValue;

            break;
        case pixelIndex: {
            unsigned short int xCalc = ( value & 0xffff0000 ) >> 16;
            unsigned short int yCalc = value & 0x0000ffff;
            int pixelIndex =
                game->calculations.find(yCalc)->second.result() *
                game->systemConfig->screenWidth +
                game->calculations.find(xCalc)->second.result();
            if (pixelIndex > 0 && pixelIndex < game->systemConfig->screenWidth * game->systemConfig->screenHeight) {
                game->pixels[pixelIndex] = newValue;
            }
            break;
        }
        case list: {
            unsigned short int listID = ( value & 0xffff0000 ) >> 16;
            unsigned short int listPosCalcID = value & 0x0000ffff;
            int listPos = game->calculations.find(listPosCalcID)->second.result();
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<unsigned short int, Value>::iterator it = values.first;
            std::advance( it, listPos );
            Value listValue = game->values.find(it->second.ID)->second;

            if (listValue.type == integer || listValue.type == pixelIndex) {
                listValue.set(newValue);
            }
            break;
        }
        default:
            return;
    };
};
