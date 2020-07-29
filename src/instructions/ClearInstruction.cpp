#include "ClearInstruction.h"
#include "Arcadable.h"

ClearInstruction::ClearInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::Clear) { }
ClearInstruction::ClearInstruction() {}


void ClearInstruction::execute() {
    Arcadable::getInstance()->canvas->fillScreen(CRGB::Black);
}

void ClearInstruction::init(std::vector<unsigned short> ids) { }