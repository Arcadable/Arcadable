#ifndef _ARC_RUNNER
#define _ARC_RUNNER

#include <Arduino.h>
#include "configuration.h"
#include "callStack.h"

class Runner {
  public: 
    CallStack callStack;
    GameState *gameState;

    Runner() {

    }
    virtual void init(GameState *gameState) {
      this->gameState = gameState;
    }

    void _processCallStack(CallStack* callStack) {
		if(callStack->size() > 0) {
			unsigned int executable = callStack->pop();
			Executable* exec = &Executable::executableStorage.find(executable)->second;

			exec->checkWaitMillis(this->gameState);
			if(exec->executeOnMillis > 0) {
				if(exec->executeOnMillis <= millis()) {
					this->_processExecutable(executable, callStack);
				} else {
					callStack->delayScheduledSection(executable);
				}
			} else {
				this->_processExecutable(executable, callStack);
			}

			if (callStack->doProcessMore()) {
				this->_processCallStack(callStack);
			}
		}
    }

    void _processExecutable(unsigned int executable, CallStack* callStack);
		
};

#endif