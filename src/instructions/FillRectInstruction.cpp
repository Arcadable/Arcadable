#include "FillRectInstruction.h"
#include "Arcadable.h"

FillRectInstruction::FillRectInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::FillRect) {
}
FillRectInstruction::FillRectInstruction() {}

void FillRectInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->x1Value = Arcadable::getInstance()->values.find(ids[1])->second;
    this->y1Value = Arcadable::getInstance()->values.find(ids[2])->second;
    this->x2Value = Arcadable::getInstance()->values.find(ids[3])->second;
    this->y2Value = Arcadable::getInstance()->values.find(ids[4])->second; 
}
void FillRectInstruction::execute() {
  
    int pixel1X = static_cast<int>(this->x1Value->getNumber());
    int pixel1Y = static_cast<int>(this->y1Value->getNumber());
    int pixel2X = static_cast<int>(this->x2Value->getNumber());
    int pixel2Y = static_cast<int>(this->y2Value->getNumber());
    int w = pixel2X - pixel1X;
    int h = pixel2Y - pixel1Y;
    CRGB drawRectColor = CRGB(this->colorValue->getNumber());
    Arcadable::getInstance()->canvas->fillRect(pixel1X, pixel1Y, w, h, drawRectColor);
}
