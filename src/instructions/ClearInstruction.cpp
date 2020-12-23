#include "ClearInstruction.h"
#include "Arcadable.h"

ClearInstruction::ClearInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::Clear, await) { }
ClearInstruction::ClearInstruction() {}

void ClearInstruction::init(std::vector<unsigned short> ids) {
    this->executables = {Executable([this] () -> const std::vector<Executable>& {
        Arcadable::getInstance()->canvas->fillScreen(CRGB::Black);
        return this->empty;
    }, false, false, NULL, NULL)};
}