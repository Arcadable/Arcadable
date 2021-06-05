#include "executable.h"
#include "gameState.h"

std::map<unsigned int, Executable> Executable::executableStorage = {};
std::vector<unsigned int> Executable::singleUseless = {Executable::createExecutable(65534, false, false, 4294967295, 65535, false)};
std::vector<unsigned int> Executable::empty = {};
std::list<unsigned int> Executable::freeIds = {};
std::map<unsigned int, unsigned int> Executable::usedIds = {};
unsigned int Executable::lastId = 0;
void Executable::checkWaitMillis(GameState *gameState) {
    if(this->_hasAwaitable && this->executeOnMillis == 0) {

        auto instr = gameState->instructions.find(this->waitDurationInstructionID)->second;
        double extraMillis = 0;
        switch(instr->type) {
            case InstructionType::Wait: {
                WaitInstruction* a = static_cast<WaitInstruction*>(instr);
                extraMillis = a->getWaitAmount();
                break;
            }
            case InstructionType::Tone: {
                ToneInstruction* a = static_cast<ToneInstruction*>(instr);
                extraMillis = a->getWaitAmount();
                break;
            }
            default: {
                break;
            }
        }

        if(extraMillis > 0) {
            this->executeOnMillis = millis() + extraMillis;
        } else {
            this->executeOnMillis = 0;
        }
        this->_hasAwaitable = false;
    }
}

            