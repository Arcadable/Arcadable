#include "DrawTriangleInstruction.h"
#include <Arcadable.h>

DrawTriangleInstruction::DrawTriangleInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawTriangle, await) {}
DrawTriangleInstruction::DrawTriangleInstruction() {}

void DrawTriangleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->x1Value = Arcadable::getInstance()->values.find(ids[1])->second;
    this->y1Value = Arcadable::getInstance()->values.find(ids[2])->second;
    this->x2Value = Arcadable::getInstance()->values.find(ids[3])->second;
    this->y2Value = Arcadable::getInstance()->values.find(ids[4])->second; 
    this->x3Value = Arcadable::getInstance()->values.find(ids[5])->second;
    this->y3Value = Arcadable::getInstance()->values.find(ids[6])->second;

        
    this->executables = {Executable([this] () -> const std::vector<Executable>& {

        int pixel1X = static_cast<int>(this->x1Value->getNumber());
        int pixel1Y = static_cast<int>(this->y1Value->getNumber());
        int pixel2X = static_cast<int>(this->x2Value->getNumber());
        int pixel2Y = static_cast<int>(this->y2Value->getNumber());
        int pixel3X = static_cast<int>(this->x3Value->getNumber());
        int pixel3Y = static_cast<int>(this->y3Value->getNumber());
        CRGB triangleColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->drawTriangle(pixel1X, pixel1Y, pixel2X, pixel2Y, pixel3X, pixel3Y, triangleColor);
        return this->empty;
    }, false, false, NULL, NULL)};
}
