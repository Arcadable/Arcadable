#include "mainRunner.h"
#include "gameState.h"

FASTRUN void MainRunner::_mainTrigger() {
    this->mainDelta = 0;
    this->digitalReader.updateValues();
    this->analogReader.updateValues();
    if(this->gameLoaded) {
        this->callStack.prepareStep();
        this->callStack.pushfront(this->gameState->mainInstructionSet->getExecutables());
        if(this->first) {
            this->callStack.pushfront(this->gameState->setupInstructionSet->getExecutables());
            this->first = false;
        }
        this->_processCallStack(&this->callStack);
    }
    SoundController::sendSounds();
}