#include "DrawTextInstruction.h"
#include <Arcadable.h>

DrawTextInstruction::DrawTextInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::DrawText) {}
DrawTextInstruction::DrawTextInstruction() {}

void DrawTextInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->scaleValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->textValue = Arcadable::getInstance()->values.find(ids[2])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[3])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[4])->second;
}
void DrawTextInstruction::execute() {
  
    int scaleValue = static_cast<int>(this->scaleValue->getNumber());
    int xValue = static_cast<int>(this->xValue->getNumber());
    int yValue = static_cast<int>(this->yValue->getNumber());
    std::vector<unsigned short>* textValue = this->textValue->getValueArray();

    CRGB textColor = CRGB(this->colorValue->getNumber());

    std::vector<char> text;
    for ( auto &value : *textValue ) {
        char c = static_cast<int>(Arcadable::getInstance()->values[value]->getNumber()) & 0x000000FF;
        text.push_back(c);
	}

    unsigned short int extraChars = (Arcadable::getInstance()->systemConfig->screenWidth - (scaleValue * 6 * text.size())) / (scaleValue * 6);
    for (int i = 0; i <= extraChars; i++ ) {
        text.push_back(' ');
    }

    Arcadable::getInstance()->canvas->setCursor(xValue, yValue);
    Arcadable::getInstance()->canvas->setTextColor(textColor);
    Arcadable::getInstance()->canvas->setTextSize(scaleValue);
    Arcadable::getInstance()->canvas->setTextWrap(false);
    Arcadable::getInstance()->canvas->print(text.data());
}