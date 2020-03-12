#include "Value.h"
#include "Arduino.h"

Value::Value(
    unsigned short ID,
    ValueType type,
    double value,
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

double Value::get() {

    switch(type) {
        case number:
        case text: {
            return value;
        }
        case pixelIndex: {
            unsigned short int xCalc = ( static_cast<int>(value) & 0xffff0000 ) >> 16;
            unsigned short int yCalc = static_cast<int>(value) & 0x0000ffff;

            int pixelIndex =
                static_cast<int>(game->calculations.find(yCalc)->second.result()) *
                game->systemConfig->screenWidth +
                static_cast<int>(game->calculations.find(xCalc)->second.result());
            return (game->pixelsBuffer[pixelIndex].r << 16) + (game->pixelsBuffer[pixelIndex].g << 8) + game->pixelsBuffer[pixelIndex].b;
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
        case systemPointer: {
            return game->systemConfig->expandedProperties[static_cast<int>(value)];
        }
        case currentTime:
            return millis() - game->startMillis;
        case list: {
            unsigned short int listID = ( static_cast<int>(value) & 0xffff0000 ) >> 16;
            unsigned short int listPosCalcID = static_cast<int>(value) & 0x0000ffff;
            int listPos = static_cast<int>(game->calculations.find(listPosCalcID)->second.result());
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

void Value::set(double newValue) {
    switch(type) {
        case number: {
            value = newValue;
            break;
        }
        case text: {
            value = static_cast<int>(newValue);
            break;
        }
        case pixelIndex: {
            unsigned short int xCalc = ( static_cast<int>(value) & 0xffff0000 ) >> 16;
            unsigned short int yCalc = static_cast<int>(value) & 0x0000ffff;

            int pixelIndex =
                static_cast<int>(game->calculations.find(yCalc)->second.result()) *
                game->systemConfig->screenWidth +
                static_cast<int>(game->calculations.find(xCalc)->second.result());
            if (pixelIndex > 0 && pixelIndex < game->systemConfig->screenWidth * game->systemConfig->screenHeight) {
                game->pixels[pixelIndex] = static_cast<int>(newValue);
            }
            break;
        }
        case list: {
            unsigned short int listID = ( static_cast<int>(value) & 0xffff0000 ) >> 16;
            unsigned short int listPosCalcID = static_cast<int>(value) & 0x0000ffff;

            int listPos = static_cast<int>(game->calculations.find(listPosCalcID)->second.result());
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::multimap<unsigned short int, Value>::iterator it = values.first;
            std::advance( it, listPos );
            Value *listValue = &game->values.find(it->second.ID)->second;

            if (
                listValue->type == number ||
                listValue->type == pixelIndex ||
                listValue->type == list ||
                listValue->type == text
            ) {
                listValue->set(newValue);
            }
            break;
        }
        default:
            return;
    };
};
