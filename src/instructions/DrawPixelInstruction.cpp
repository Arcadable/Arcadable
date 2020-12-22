#include "DrawPixelInstruction.h"
#include "Arcadable.h"

DrawPixelInstruction::DrawPixelInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawPixel, await) {}
DrawPixelInstruction::DrawPixelInstruction() {}

void DrawPixelInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[2])->second;
}
std::vector<Executable>* DrawPixelInstruction::getExecutables(bool async) {
  
    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        int pixelX = static_cast<int>(this->xValue->getNumber());
        int pixelY = static_cast<int>(this->yValue->getNumber());

        CRGB pixelColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->drawPixel(pixelX, pixelY, pixelColor);
        return {};


    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}
