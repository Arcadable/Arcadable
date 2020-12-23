#include "DrawCircleInstruction.h"
#include "Arcadable.h"

DrawCircleInstruction::DrawCircleInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DrawCircle, await) {}
DrawCircleInstruction::DrawCircleInstruction() {}

void DrawCircleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->radiusValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[2])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[3])->second;

    this->executables = {Executable([this] () -> const std::vector<Executable>& {
        int radiusValue = static_cast<int>(this->radiusValue->getNumber());
        int xValue = static_cast<int>(this->xValue->getNumber());
        int yValue = static_cast<int>(this->yValue->getNumber());

        CRGB drawCircleColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->drawCircle(xValue, yValue, radiusValue, drawCircleColor);
        return this->empty;
    }, false, false, NULL, NULL)};
}
