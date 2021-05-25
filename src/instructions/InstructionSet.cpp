#include "InstructionSet.h"
#include "../executable.h"
#include "../gameState.h"

InstructionSet::InstructionSet (
    unsigned short ID,
    unsigned short size,
    bool async,
    GameState *game
) {
    this->size = size;
    this->ID = ID;
    this->instructions = std::vector<unsigned short>(this->size);
    this->async = async;
    this->executables = {};
    this->processedExecutables = {};
    this->game = game;
}

InstructionSet::InstructionSet() {}
void InstructionSet::init(std::vector<unsigned short> ids) {

    this->instructions = ids;

}
std::vector<unsigned int>* InstructionSet::getExecutables() {

    this->executables.clear();

    for ( auto instruction : this->instructions ) {
        auto instructionRef = this->game->instructions[instruction];

        switch(this->game->instructions[instruction]->type) {
            case InstructionType::Wait: {
                this->executables.push_back(Executable::createExecutable(instructionRef->ID, this->async, instructionRef->await, 4294967295, instructionRef->ID, true));
                break;
            }
            case InstructionType::RunCondition: {
                this->executables.push_back(Executable::createExecutable(instructionRef->ID, this->async, this->async, 4294967295, 65535, true));
                break;
            }
            case InstructionType::Tone: {
                if(instructionRef->await) {
                    this->executables.push_back(Executable::createExecutable(instructionRef->ID, this->async, false, 4294967295, 65535, true));
                    this->executables.push_back(Executable::createExecutable(65535, this->async, true, 4294967295, instructionRef->ID, false));
                } else {
                    this->executables.push_back(Executable::createExecutable(instructionRef->ID, this->async, false, 4294967295, 65535, true));
                }

                break;
            }
            default: {
                this->executables.push_back(Executable::createExecutable(instructionRef->ID, this->async, instructionRef->await, 4294967295, 65535, true));
                break;
            }
        }
	}

    this->_processAwaiting(&this->executables, &this->processedExecutables);

    return &this->executables;
}
void InstructionSet::_processAwaiting(std::vector<unsigned int>* executables, std::vector<unsigned int>* out) {

    Executable* awaitExecutable = NULL;
    for ( auto executable : *executables ) {
        Executable* exec = &Executable::executableStorage.find(executable)->second;

        if(awaitExecutable != NULL) {
            awaitExecutable->awaiting.push_back(executable);
        } else {
            if(exec->await) {
                awaitExecutable = exec;
            }
            out->push_back(executable);
        }
    }
    executables->clear();
    for ( auto executable : *out ) {
        executables->push_back(executable);
    }
    out->clear();
    if(awaitExecutable != NULL && awaitExecutable->awaiting.size() > 0) {

        this->_processAwaiting(&awaitExecutable->awaiting, out);
        awaitExecutable->awaitingWaitForExec = Executable::createCustomTemporary(&awaitExecutable->awaiting, true, false, 4294967295, 65535);

    }
}
