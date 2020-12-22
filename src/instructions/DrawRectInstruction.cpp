#include "DrawRectInstruction.h"
#include "Arcadable.h"

DrawRectInstruction::DrawRectInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawRect, await) {}
DrawRectInstruction::DrawRectInstruction() {}

void DrawRectInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->x1Value = Arcadable::getInstance()->values.find(ids[1])->second;
    this->y1Value = Arcadable::getInstance()->values.find(ids[2])->second;
    this->x2Value = Arcadable::getInstance()->values.find(ids[3])->second;
    this->y2Value = Arcadable::getInstance()->values.find(ids[4])->second; 
}
std::vector<Executable>* DrawRectInstruction::getExecutables(bool async) {
  
    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        int pixel1X = static_cast<int>(this->x1Value->getNumber());
        int pixel1Y = static_cast<int>(this->y1Value->getNumber());
        int pixel2X = static_cast<int>(this->x2Value->getNumber());
        int pixel2Y = static_cast<int>(this->y2Value->getNumber());
        int w = pixel2X - pixel1X;
        int h = pixel2Y - pixel1Y;
        CRGB drawRectColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->drawRect(pixel1X, pixel1Y, w, h, drawRectColor);
        return {};

    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}
