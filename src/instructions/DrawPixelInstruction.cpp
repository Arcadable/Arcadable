#include "DrawPixelInstruction.h"
#include "Arcadable.h"

DrawPixelInstruction::DrawPixelInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::DrawPixel) {}
DrawPixelInstruction::DrawPixelInstruction() {}

void DrawPixelInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[2])->second;
}
void DrawPixelInstruction::execute() {
  
    int pixelX = static_cast<int>(this->xValue->getNumber());
    int pixelY = static_cast<int>(this->yValue->getNumber());

    CRGB pixelColor = CRGB(this->colorValue->getNumber());
    Arcadable::getInstance()->canvas->drawPixel(pixelX, pixelY, pixelColor);
}
