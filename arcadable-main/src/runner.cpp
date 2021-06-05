#include "runner.h"
#include "gameState.h"

void Runner::_processExecutable(unsigned int executable, CallStack* callStack) {
    Executable* exec = &Executable::executableStorage.find(executable)->second;

    std::vector<unsigned int>* newExecutables = {};
    
    if(exec->returnExecutables.size() > 0) {
        newExecutables = &exec->returnExecutables;
    } else if(exec->executeInstructionID == 65535) {
        newExecutables = &Executable::singleUseless;
    } else if(exec->executeInstructionID == 65534) {
        newExecutables = &Executable::empty;
    } else if (exec->deep == true) {
        newExecutables = this->gameState->instructions.find(exec->executeInstructionID)->second->deepAction(exec->async);
    } else {
        newExecutables = this->gameState->instructions.find(exec->executeInstructionID)->second->action(exec->async);
    }
    Executable::freeExecutable(executable);

    if(exec->parentAwait != 4294967295) {
        for ( auto item : *newExecutables ) {
            Executable* newExec = &Executable::executableStorage.find(item)->second;
            newExec->withParentAwait(exec->parentAwait);
        }
    }

    if(newExecutables->size() > 0) {
        if(exec->async) {
            if (exec->awaiting.size() > 0) {

                if(exec->parentAwait != 4294967295) {
                    for ( auto item : exec->awaiting ) {
                        Executable::executableStorage.find(item)->second.withParentAwait(exec->parentAwait);
                    }
                    Executable::executableStorage.find(exec->awaitingWaitForExec)->second.parentAwait = exec->parentAwait;
                }
                                            
                for ( auto item : *newExecutables ) {
                    Executable* newExec = &Executable::executableStorage.find(item)->second;
                    newExec->withParentAwait(exec->awaitingWaitForExec);

                }
                callStack->pushfront(newExecutables);

                if(exec->parentAwait != 4294967295) {
                    callStack->pushinfrontof(exec->parentAwait, exec->awaitingWaitForExec);
                } else {
                    callStack->pushback(exec->awaitingWaitForExec);
                }
            } else {
                callStack->pushfront(newExecutables);
            }
        } else {
            callStack->pushfront(newExecutables);
        }
    
    }
}