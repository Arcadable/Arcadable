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
    this->frequencyValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->durationValue = Arcadable::getInstance()->values.find(ids[2])->second;
    
    this->executables = {Executable([this] () -> const std::vector<Executable>& {

        double volume = this->volumeValue->getNumber();
        double frequency = this->frequencyValue->getNumber();
        double duration = this->durationValue->getNumber();

        analogWrite(output, 127);
        Wait for 1,136 microseconds
        analogWrite(output, 0);
        Wait for 1,136 microseconds

        return this->empty;


    }, false, false, NULL, NULL)};
}
