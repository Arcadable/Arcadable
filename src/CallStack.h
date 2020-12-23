#pragma once
#define CallStack_h
#include <vector>
#include <list>
#include <functional>
#include "Arduino.h"
#include "Executable.h"
class CallStack {
    public:
        std::list<Executable*> storage;
        unsigned long int delayed; 
        unsigned long int capacity; 

        CallStack();
        CallStack(
            unsigned long int capacity
        );

        void checkWaitMillis();
        Executable* withParentAwait(Executable* p);
    	void pushback(std::vector<Executable>* items);
        void pushinfrontof(Executable* infrontof, std::vector<Executable>* items);
        void pushfront(std::vector<Executable>* items);
        void delayScheduledSection(Executable* parent);
        bool doProcessMore();
        void prepareStep();
        Executable* pop();
        unsigned long int size();


};
