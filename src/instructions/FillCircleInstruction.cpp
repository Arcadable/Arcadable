#include "FillCircleInstruction.h"
#include "Arcadable.h"

FillCircleInstruction::FillCircleInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::FillCircle) {}
FillCircleInstruction::FillCircleInstruction() {}

void FillCircleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->radiusValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[2])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[3])->second;
}
void FillCircleInstruction::execute() {
  
    int radiusValue = static_cast<int>(this->radiusValue->getNumber());
    int xValue = static_cast<int>(this->xValue->getNumber());
    int yValue = static_cast<int>(this->yValue->getNumber());

    CRGB drawCircleColor = CRGB(this->colorValue->getNumber());
    Arcadable::getInstance()->canvas->fillCircle(xValue, yValue, radiusValue, drawCircleColor);
}
