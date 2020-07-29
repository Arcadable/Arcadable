#include "DebugLogInstruction.h"
#include "Arduino.h"
#include "values/ListValue.h"
#include "Arcadable.h"
DebugLogInstruction::DebugLogInstruction (
    unsigned short ID
) : Instruction(ID, InstructionType::DebugLog) {}
DebugLogInstruction::DebugLogInstruction() {}

void DebugLogInstruction::init(std::vector<unsigned short> ids) {
    this->logValue = Arcadable::getInstance()->values.find(ids[0])->second;
}
void DebugLogInstruction::execute() {
    Serial.print("debug.log - ");
    this->print(this->logValue);
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