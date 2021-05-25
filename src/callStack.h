#ifndef ARC_CALLSTACK
#define ARC_CALLSTACK
#include <vector>
#include <list>
#include "Arduino.h"
#include "executable.h"
class CallStack {
    public:
        std::list<unsigned int> storage;
        unsigned int delayed; 
        unsigned int capacity; 

        CallStack() {
            this->capacity = 4294967295;
        }
        CallStack(
            unsigned int capacity
        ) {
            this->capacity = capacity;
            if(this->capacity <= 0) {
                this->capacity = 4294967295;
            }
        }

        unsigned int withParentAwait(unsigned int p);
    	void pushback(std::vector<unsigned int>* items) {
            if (this->size() >= this->capacity) {
                Serial.println("Stack has reached max capacity. 1");
            }
            for ( auto item : *items ) {
                this->storage.push_back(item);
            }
        }
        void pushinfrontof(unsigned int infrontof, std::vector<unsigned int>* items) {
            if (this->size() >= this->capacity) {
                Serial.println("Stack has reached max capacity. 2");
            }
            std::list<unsigned int>::iterator it;

            it = this->storage.begin();
            for(auto executable : this->storage) {
                if(executable == infrontof) {
                    break;
                } else {
                    ++it; 
                }
            }

            for ( auto item : *items ) {
                this->storage.insert(it, item);
            }
        }
        void pushback(unsigned int item) {
            if (this->size() >= this->capacity) {
                Serial.println("Stack has reached max capacity. 3");
            }
            this->storage.push_back(item);
        }
        void pushinfrontof(unsigned int infrontof, unsigned int item) {
            if (this->size() >= this->capacity) {
                Serial.println("Stack has reached max capacity. 4");
            }
            std::list<unsigned int>::iterator it;

            it = this->storage.begin();
            for(auto executable : this->storage) {
                if(executable == infrontof) {
                    break;
                } else {
                    ++it; 
                }
            }

            this->storage.insert(it, item);
        }
        void pushfront(std::vector<unsigned int>* items) {
            if (this->size() >= this->capacity) {
                Serial.println("Stack has reached max capacity. 5");
            }
            std::list<unsigned int>::iterator it;
            it = this->storage.begin();
            for ( auto item : *items ) {
                this->storage.insert(it, item);
            }
        }
        void delayScheduledSection(unsigned int parent) {
            this->storage.remove(parent);
            this->pushback(parent);
            this->delayed++;
            Executable* exec = &Executable::executableStorage.find(parent)->second;
            if(exec->parentAwait != 4294967295) {
                this->delayScheduledSection(exec->parentAwait);
            }
        }
        bool doProcessMore() {
            return this->delayed < this->size();
        }
        void prepareStep() {
            this->delayed = 0;
        }
        unsigned int pop() {
            unsigned int front = this->storage.front();
            this->storage.pop_front();
            return front;
        }
        unsigned int size() {
            return this->storage.size();
        }
        void clear() {
            this->storage.clear();
        }


};
#endif