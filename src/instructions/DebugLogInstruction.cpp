#include "DebugLogInstruction.h"
#include "Arduino.h"
#include "../values/ListValue.h"
#include "../executable.h"
#include "../gameState.h"
DebugLogInstruction::DebugLogInstruction (
    unsigned short ID,
    bool await,
    GameState *game
) : Instruction(ID, InstructionType::DebugLog, await) {
    this->game = game;
}
DebugLogInstruction::DebugLogInstruction() {}

void DebugLogInstruction::init(std::vector<unsigned short> ids) {
    this->logValue = this->game->values.find(ids[0])->second;

    
}

void DebugLogInstruction::print(Value *v) {
    if(v->isNumberType) {
        Serial.println(v->getNumber());
    } else {
        Serial.print("[");
        for (auto &v2 : *v->getValueArray()) {
            this->print(this->game->values[v2]);
            Serial.print(", ");
        }
        Serial.print("]");
        Serial.println("");
    }
}
std::vector<unsigned int>* DebugLogInstruction::action(bool async) {
    Serial.print("debug.log - ");
    this->print(this->logValue);
    return &Executable::empty;



}
double DebugLogInstruction::getWaitAmount() {
    return 0;
}
