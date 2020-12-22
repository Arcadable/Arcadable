#include "WaitInstruction.h"
#include "Arduino.h"
#include "values/ListValue.h"
#include "Arcadable.h"
WaitInstruction::WaitInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::Wait, await) {}
WaitInstruction::WaitInstruction() {}

void WaitInstruction::init(std::vector<unsigned short> ids) {
    this->amountValue = Arcadable::getInstance()->values.find(ids[0])->second;
}
std::vector<Executable>* WaitInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {
        return {};
    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}
