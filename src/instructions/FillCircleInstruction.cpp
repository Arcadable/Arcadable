#include "FillCircleInstruction.h"
#include "Arcadable.h"

FillCircleInstruction::FillCircleInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::FillCircle, await) {}
FillCircleInstruction::FillCircleInstruction() {}

void FillCircleInstruction::init(std::vector<unsigned short> ids) {
    this->colorValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->radiusValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->xValue = Arcadable::getInstance()->values.find(ids[2])->second;
    this->yValue = Arcadable::getInstance()->values.find(ids[3])->second;
}
std::vector<Executable>* FillCircleInstruction::getExecutables(bool async) {
  
    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        int radiusValue = static_cast<int>(this->radiusValue->getNumber());
        int xValue = static_cast<int>(this->xValue->getNumber());
        int yValue = static_cast<int>(this->yValue->getNumber());

        CRGB drawCircleColor = CRGB(this->colorValue->getNumber());
        Arcadable::getInstance()->canvas->fillCircle(xValue, yValue, radiusValue, drawCircleColor);
        return {};
    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}
