#ifndef _ARC_EXECUTABLE
#define _ARC_EXECUTABLE
#include <vector>
#include "Arduino.h"
#include "./instructions/Instruction.h"
#include <map>
#include <list>
class GameState;

class Executable {
    public:
        static unsigned int lastId;
        static std::map<unsigned int, Executable> executableStorage;
        static std::vector<unsigned int> singleUseless;
        static std::vector<unsigned int> empty;
        static std::list<unsigned int> freeIds;
        static std::map<unsigned int, unsigned int> usedIds;

        bool free;
        unsigned int ID;
        bool async;
        bool await;
        std::vector<unsigned int> awaiting;
        unsigned int parentAwait;
        unsigned short waitDurationInstructionID;
        unsigned short executeInstructionID;
        bool deep;
        bool temporary;
        unsigned long int executeOnMillis;
        std::vector<unsigned int> returnExecutables;
        unsigned int awaitingWaitForExec;
        Executable() {}
        Executable(
            unsigned int ID,
            unsigned short executeInstructionID,
            bool async,
            bool await,
            unsigned int parentAwait,
            unsigned short waitDurationInstructionID,
            bool temporary,
            bool deep,
            std::vector<unsigned int>* r
        ) {
            this->ID = ID;
            this->executeInstructionID = executeInstructionID;
            this->async = async;
            this->await = await;
            this->awaiting = {};
            this->parentAwait = parentAwait;
            this->waitDurationInstructionID = waitDurationInstructionID;
            this->temporary = temporary;
            this->deep = deep;
            this->returnExecutables = {};
            if(r != NULL) {
                for(auto item : *r) {
                    this->returnExecutables.push_back(item);
                }
            }
            this->free = false;
            this->resetCheckWaitMillis();

        }
        static unsigned int createExecutable(
            unsigned short executeInstructionID,
            bool async,
            bool await,
            unsigned int parentAwait,
            unsigned short waitDurationInstructionID,
            bool deep
        ) {
            unsigned int newID = Executable::getNewId();
            Executable::executableStorage[newID] = Executable(newID, executeInstructionID, async, await, parentAwait, waitDurationInstructionID, false, deep, NULL);
            return newID;
        }
        static unsigned int createCustomTemporary(
            std::vector<unsigned int>* returnExecutables,
            bool async,
            bool await,
            unsigned int parentAwait,
            unsigned short waitDurationInstructionID
        ) {
            unsigned int newID = Executable::getNewId();
            Executable::executableStorage[newID] = Executable(newID, 65535, async, await, parentAwait, 65535, true, false, returnExecutables);
            return newID;
        }
        static unsigned int getNewId() {
            unsigned int newID;
            if(Executable::freeIds.size() > 0) {
                newID = Executable::freeIds.front();
                Executable::freeIds.pop_front();
            } else {
                Executable::lastId++;
                newID = Executable::lastId;
            }
            return newID;
        }
        static void freeExecutable(unsigned int executable) {
            if(Executable::executableStorage[executable].free == false) {
                Executable::freeIds.push_back(executable);
                Executable::executableStorage[executable].free = true;
            }
        }

        void checkWaitMillis(GameState *gameState);
        void resetCheckWaitMillis() {
            if(this->waitDurationInstructionID != 65535) {
                this->_hasAwaitable = true;
            } else {
                this->_hasAwaitable = false;
            }
            this->executeOnMillis = 0;
        }

        unsigned int withParentAwait(unsigned int p) {
            this->parentAwait = p;
            return this->ID;
        }
    private:
        bool _hasAwaitable = false;

};

#endif