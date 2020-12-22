#include "ClearInstruction.h"
#include "Arcadable.h"

ClearInstruction::ClearInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::Clear, await) { }
ClearInstruction::ClearInstruction() {}


std::vector<Executable>* ClearInstruction::getExecutables(bool async) {
    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([] () -> const std::vector<Executable>& {
        Arcadable::getInstance()->canvas->fillScreen(CRGB::Black);
        return {};
    }, async, false, awaiting, NULL, NULL)};
    return &executables;
}

void ClearInstruction::init(std::vector<unsigned short> ids) { }