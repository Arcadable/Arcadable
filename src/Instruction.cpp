#include "Condition.h"
#include "Value.h"
#include "Arduino.h"

Instruction::Instruction(
    unsigned short ID,
    unsigned int leftID,
    bool rightIsValue,
    unsigned int rightID,
    InstructionType instructionType
) {
    this->ID = ID;
    this->leftID = leftID;
    this->rightIsValue = rightIsValue;
    this->rightID = rightID;
    this->instructionType = instructionType;

    game = Arcadable::getInstance();
};

void Instruction::execute() {

    switch(instructionType) {
        case MutateValue: {
            Value *valueLeft = &game->values.find(leftID)->second;
            if (
                valueLeft->type == integer ||
                valueLeft->type == pixelIndex ||
                valueLeft->type == list ||
                valueLeft->type == text
            ) {
                int right = rightIsValue ? game->values.find(rightID)->second.get() : game->calculations.find(rightID)->second.result();
                valueLeft->set(right);
            }

            break;
        }
        case RunCondition: {
            game->conditions.find(leftID)->second.execute();
            break;
        }
        case DrawPixel: {
            unsigned short int xCalc = ( game->values.find(leftID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int yCalc = ( game->values.find(leftID)->second.value & 0x0000ffff );
            int x = game->calculations.find(xCalc)->second.result();
            int y = game->calculations.find(yCalc)->second.result();
            CRGB pixelColor = CRGB(rightIsValue ?
                game->values.find(rightID)->second.get() :
                game->calculations.find(rightID)->second.result()
            );
            game->canvas->drawPixel(x, y, pixelColor);
            break;
        }
        case DrawLine: {
            unsigned short int pos1ID = (leftID & 0xffff0000) >> 16;
            unsigned short int pos2ID = leftID & 0x0000ffff;

            unsigned short int pos1XCalc = ( game->values.find(pos1ID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pos1YCalc = ( game->values.find(pos1ID)->second.value & 0x0000ffff );

            unsigned short int pos2XCalc = ( game->values.find(pos2ID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pos2YCalc = ( game->values.find(pos2ID)->second.value & 0x0000ffff );

            int pos1X = game->calculations.find(pos1XCalc)->second.result();
            int pos1Y = game->calculations.find(pos1YCalc)->second.result();

            int pos2X = game->calculations.find(pos2XCalc)->second.result();
            int pos2Y = game->calculations.find(pos2YCalc)->second.result();

            CRGB lineColor = CRGB(rightIsValue ?
                game->values.find(rightID)->second.get() :
                game->calculations.find(rightID)->second.result()
            );
            game->canvas->drawLine(pos1X, pos1Y, pos2X, pos2Y, lineColor);
            break;
        }
        case DrawRect:
        case FillRect: {
            unsigned short int topLeftDrawID = (leftID & 0xffff0000) >> 16;
            unsigned short int bottomRightDrawID = leftID & 0x0000ffff;

            unsigned short int topLeftDrawXCalc = ( game->values.find(topLeftDrawID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int topLeftDrawYCalc = ( game->values.find(topLeftDrawID)->second.value & 0x0000ffff );

            unsigned short int bottomRightDrawXCalc = ( game->values.find(bottomRightDrawID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int bottomRightDrawYCalc = ( game->values.find(bottomRightDrawID)->second.value & 0x0000ffff );

            int topLeftDrawX = game->calculations.find(topLeftDrawXCalc)->second.result();
            int topLeftDrawY = game->calculations.find(topLeftDrawYCalc)->second.result();

            int bottomRightDrawX = game->calculations.find(bottomRightDrawXCalc)->second.result();
            int bottomRightDrawY = game->calculations.find(bottomRightDrawYCalc)->second.result();

            int width = bottomRightDrawX - topLeftDrawX;
            int height = bottomRightDrawY - topLeftDrawY;

            CRGB drawRectColor = CRGB(rightIsValue ?
                game->values.find(rightID)->second.get() :
                game->calculations.find(rightID)->second.result()
            );

            if (instructionType == InstructionType::DrawRect) {
                game->canvas->drawRect(topLeftDrawX, topLeftDrawY, width, height, drawRectColor);
            } else {
                game->canvas->fillRect(topLeftDrawX, topLeftDrawY, width, height, drawRectColor);
            }
            break;
        }

        case DrawCircle:
        case FillCircle: {
            unsigned short int centerID = (leftID & 0xffff0000) >> 16;
            unsigned short int radiusCalcID = leftID & 0x0000ffff;
            unsigned short int centerXCalcID = ( game->values.find(centerID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int centerYCalcID = ( game->values.find(centerID)->second.value & 0x0000ffff );
            int radius = game->calculations.find(radiusCalcID)->second.result();
            int centerX = game->calculations.find(centerXCalcID)->second.result();
            int centerY = game->calculations.find(centerYCalcID)->second.result();
            CRGB drawCircleColor = CRGB(rightIsValue ?
                game->values.find(rightID)->second.get() :
                game->calculations.find(rightID)->second.result()
            );

            if (instructionType == InstructionType::DrawCircle) {
                game->canvas->drawCircle(centerX, centerY, radius, drawCircleColor);
            } else {
                game->canvas->fillCircle(centerX, centerY, radius, drawCircleColor);
            }
            break;
        }
        case DrawTriangle:
        case FillTriangle: {
            unsigned short int pixel1DrawID = (leftID & 0xffff0000) >> 16;
            unsigned short int pixel2DrawID = leftID & 0x0000ffff;
            unsigned short int pixel3DrawID = (rightID & 0xffff0000) >> 16;
            CRGB triangleColor = CRGB(rightIsValue ?
                game->values.find(rightID & 0x0000ffff)->second.get() :
                game->calculations.find(rightID & 0x0000ffff)->second.result()
            );
            unsigned short int pixel1XCalc = ( game->values.find(pixel1DrawID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pixel1YCalc = ( game->values.find(pixel1DrawID)->second.value & 0x0000ffff );
            unsigned short int pixel2XCalc = ( game->values.find(pixel2DrawID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pixel2YCalc = ( game->values.find(pixel2DrawID)->second.value & 0x0000ffff );
            unsigned short int pixel3XCalc = ( game->values.find(pixel3DrawID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pixel3YCalc = ( game->values.find(pixel3DrawID)->second.value & 0x0000ffff );
            int pixel1X = game->calculations.find(pixel1XCalc)->second.result();
            int pixel1Y = game->calculations.find(pixel1YCalc)->second.result();
            int pixel2X = game->calculations.find(pixel2XCalc)->second.result();
            int pixel2Y = game->calculations.find(pixel2YCalc)->second.result();
            int pixel3X = game->calculations.find(pixel3XCalc)->second.result();
            int pixel3Y = game->calculations.find(pixel3YCalc)->second.result();
            if (instructionType == InstructionType::DrawTriangle) {
                game->canvas->drawTriangle(pixel1X, pixel1Y, pixel2X, pixel2Y, pixel3X, pixel3Y, triangleColor);
            } else {
                game->canvas->fillTriangle(pixel1X, pixel1Y, pixel2X, pixel2Y, pixel3X, pixel3Y, triangleColor);
            }
            break;
        }
        case DrawText: {
            unsigned short int pixelTextID = (leftID & 0xffff0000) >> 16;
            unsigned short int pixelTextXCalc = ( game->values.find(pixelTextID)->second.value & 0xffff0000 ) >> 16;
            unsigned short int pixelTextYCalc = ( game->values.find(pixelTextID)->second.value & 0x0000ffff );
            int pixelTextX = game->calculations.find(pixelTextXCalc)->second.result();
            int pixelTextY = game->calculations.find(pixelTextYCalc)->second.result();
            int scale = game->values.find(leftID & 0x0000ffff)->second.get();
            unsigned short int listID = (rightID & 0xffff0000) >> 16;
            CRGB textColor = CRGB(rightIsValue ?
                game->values.find(rightID & 0x0000ffff)->second.get() :
                game->calculations.find(rightID & 0x0000ffff)->second.result()
            );
            std::pair<std::multimap<unsigned short int, Value>::iterator, std::multimap<unsigned short int, Value>::iterator> values;
            values = game->lists.equal_range(listID);
            std::vector<char> text;
            for (std::multimap<unsigned short int, Value>::iterator it = values.first; it != values.second; it++) {
                Value value = game->values.find(it->second.ID)->second;
                if (value.type == ValueType::text) {
                    char c = (value.value & 0xFF000000) >> 24;
                    if (c != 0) {
                        text.push_back(c);
                    }
                    c = (value.value & 0x00FF0000) >> 16;
                    if (c != 0) {
                        text.push_back(c);
                    }
                    c = (value.value & 0x0000FF00) >> 8;
                    if (c != 0) {
                        text.push_back(c);
                    }
                    c = value.value & 0x000000FF;
                    if (c != 0) {
                        text.push_back(c);
                    }
                }
            }
            unsigned short int extraChars = (game->systemConfig->screenWidth - (scale * 6 * text.size())) / (scale * 6);
            for (int i = 0; i <= extraChars; i++ ) {
                text.push_back(' ');
            }
       

            game->canvas->setCursor(pixelTextX, pixelTextY);
            game->canvas->setTextColor(textColor);
            game->canvas->setTextSize(scale);
            game->canvas->setTextWrap(false);
            game->canvas->print(text.data());
            break;
        }
        case Clear: {
            game->canvas->fillScreen(CRGB::Black);
        }
        case SetRotation: {
            game->canvas->setRotation(leftID);
        }
        break;
    }
};
