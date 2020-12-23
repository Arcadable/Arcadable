#include "DebugLogInstruction.h"
#include "Arduino.h"
#include "values/ListValue.h"
#include "Arcadable.h"
DebugLogInstruction::DebugLogInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::DebugLog, await) {}
DebugLogInstruction::DebugLogInstruction() {}

void DebugLogInstruction::init(std::vector<unsigned short> ids) {
    this->logValue = Arcadable::getInstance()->values.find(ids[0])->second;

    this->executables = {Executable([this] () -> const std::vector<Executable>& {
        Serial.print("debug.log - ");
        this->print(this->logValue);
        return this->empty;
    }, false, false, NULL, NULL)};
}

void DebugLogInstruction::print(Value *v) {
    if(v->isNumberType) {
        Serial.println(v->getNumber());
    } else {
        Serial.print("[");
        for (auto &v2 : *v->getValueArray()) {
            this->print(Arcadable::getInstance()->values[v2]);
            Serial.print(", ");
        }
        Serial.print("]");
        Serial.println("");
    }
}