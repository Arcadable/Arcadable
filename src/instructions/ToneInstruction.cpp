#include "WaitInstruction.h"
#include "Arduino.h"
#include "values/ListValue.h"
#include "Arcadable.h"
ToneInstruction::ToneInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::Tone, await) {}
ToneInstruction::ToneInstruction() {}

void ToneInstruction::init(std::vector<unsigned short> ids) {
    this->volumeValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->frequencyValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->durationValue = Arcadable::getInstance()->values.find(ids[0])->second;
}
std::vector<Executable>* ToneInstruction::getExecutables(bool async) {

    std::vector<Executable> awaiting = {};
    std::vector<Executable> executables = {Executable([this] () -> const std::vector<Executable>& {

        return {};


    }, async, false, awaiting, NULL, NULL)};

    return &executables;
}
