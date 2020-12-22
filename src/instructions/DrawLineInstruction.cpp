#include "DrawLineInstruction.h"
#include "Arcadable.h"

DrawLineInstruction::DrawLineInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawLine, await) {}
DrawLineInstruction::DrawLineInstruction() {}

void DrawLineInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->x1Value = Arcadable::getInstance()->values.find(ids[1])->second;
    this->y1Value = Arcadable::getInstance()->values.find(ids[2])->second;
    this->x2Value = Arcadable::getInstance()->values.find(ids[3])->second;
    this->y2Value = Arcadable::getInstance()->values.find(ids[4])->second; 
}
std::vector<Executable>* DrawLineInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
     std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {
        int pixel1X = static_cast<int>(this->x1Value->getNumber());
        int pixel1Y = static_cast<int>(this->y1Value->getNumber());
        int pixel2X = static_cast<int>(this->x2Value->getNumber());
        int pixel2Y = static_cast<int>(this->y2Value->getNumber());
        CRGB drawLineColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->drawLine(pixel1X, pixel1Y, pixel2X, pixel2Y, drawLineColor);
        return {};

    }, async, false, awaiting, NULL, NULL)};

    return &executables;

}
