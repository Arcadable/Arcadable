#include "InstructionSet.h"
#include "Arcadable.h"

InstructionSet::InstructionSet (
    unsigned short ID,
    unsigned short size,
    bool async
) {
    this->size = size;
    this->ID = ID;
    this->instructions = std::vector<unsigned short>(this->size);
    this->async = async;
    this->empty = {};
    this->singleUseless = {Executable([this] () -> const std::vector<Executable>& { return this->empty; }, this->async, false, NULL, NULL)};

}

InstructionSet::InstructionSet() {}
void InstructionSet::init(std::vector<unsigned short> ids) {

    this->instructions = ids;
    this->executables = {};

    for ( auto instruction : this->instructions ) {
        auto instructionRef = Arcadable::getInstance()->instructions[instruction];
        switch(Arcadable::getInstance()->instructions[instruction]->type) {
            case InstructionType::Wait: {
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    return *instructionRef->getExecutables(this->async);
                }, this->async, instructionRef->await, NULL, [instructionRef] () -> unsigned long int {
                    return static_cast<WaitInstruction*>(instructionRef)->amountValue->getNumber();
                });
                this->executables.push_back(newExecutable);
                break;
            }
            case InstructionType::Tone: {
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    return *instructionRef->getExecutables(this->async);
                }, this->async, false, NULL, NULL);
                auto toAwait = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    return this->singleUseless;
                }, this->async, true, NULL, [instructionRef] () -> unsigned long int {
                    return static_cast<ToneInstruction*>(instructionRef)->durationValue->getNumber();
                });
                this->executables.push_back(newExecutable);
                this->executables.push_back(toAwait);

                break;
            }
            default: {
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    return *instructionRef->getExecutables(this->async);
                }, this->async, instructionRef->await, NULL, NULL);
                this->executables.push_back(newExecutable);
                break;
            }
        }
	}
    std::vector<Executable> processed = {};
    this->_processAwaiting(&this->executables, &processed);
    this->executables = processed;

}
std::vector<Executable>* InstructionSet::getExecutables() {
    return &this->executables;
}
void InstructionSet::_processAwaiting(std::vector<Executable>* executables, std::vector<Executable>* out) {

    Executable* awaitExecutable = NULL;
    for ( auto &executable : *executables ) {
        if(awaitExecutable != NULL) {
            awaitExecutable->awaiting.push_back(executable);
        } else {
            if(executable.await) {
                awaitExecutable = &executable;
            }
            out->push_back(executable);
        }
    }

    if(awaitExecutable != NULL) {
        std::vector<Executable> processed = {};
        this->_processAwaiting(&awaitExecutable->awaiting, &processed);
        awaitExecutable->awaiting = processed;

    }
}
