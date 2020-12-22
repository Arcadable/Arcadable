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
}

InstructionSet::InstructionSet() {}
void InstructionSet::init(std::vector<unsigned short> ids) {

    this->instructions = ids;
}
std::vector<Executable>* InstructionSet::getExecutables() {
    std::vector<Executable> allExecutables = {}; 
    for ( auto instruction : this->instructions ) {
        auto instructionRef = Arcadable::getInstance()->instructions[instruction];
        switch(Arcadable::getInstance()->instructions[instruction]->type) {
            case InstructionType::Wait: {
                std::vector<Executable> awaiting = {};
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    instructionRef->getExecutables(this->async);
                }, this->async, instructionRef->await, awaiting, NULL, [instructionRef] () -> unsigned long int {
                    return static_cast<WaitInstruction*>(instructionRef)->amountValue->getNumber();
                });
                allExecutables.push_back(newExecutable);
                break;
            }
            case InstructionType::Tone: {
                std::vector<Executable> awaiting = {};
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    instructionRef->getExecutables(this->async);
                }, this->async, false, awaiting, NULL, NULL);
                std::vector<Executable> awaiting2 = {};
                auto toAwait = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    std::vector<Executable> awaiting3 = {};
                    return {
                        Executable([] () -> const std::vector<Executable>& { return {}; }, this->async, false, awaiting3, NULL, NULL)
                    };
                }, this->async, true, awaiting2, NULL, [instructionRef] () -> unsigned long int {
                    return static_cast<ToneInstruction*>(instructionRef)->durationValue->getNumber();
                });
                allExecutables.push_back(newExecutable);
                allExecutables.push_back(toAwait);

                break;
            }
            default: {
                std::vector<Executable> awaiting = {};
                auto newExecutable = Executable([this, instructionRef] () -> const std::vector<Executable>& {
                    instructionRef->getExecutables(this->async);
                }, this->async, instructionRef->await, awaiting, NULL, NULL);
                allExecutables.push_back(newExecutable);
                break;
            }
        }
	}

    return this->processAwaiting(allExecutables);

}

std::vector<Executable>* InstructionSet::processAwaiting(std::vector<Executable>& executables) {

    Executable* awaitExecutable = NULL;
    std::vector<Executable> processedExecutables = {}; 
    for ( auto executable : executables ) {
        if(awaitExecutable != NULL) {
            awaitExecutable->awaiting.push_back(executable);
        } else {
            if(executable.await) {
                awaitExecutable = &executable;
            }
            processedExecutables.push_back(executable);
        }
    }

    if(awaitExecutable != NULL) {
        awaitExecutable->awaiting = *this->processAwaiting(awaitExecutable->awaiting);
    }
    return &processedExecutables;
}
