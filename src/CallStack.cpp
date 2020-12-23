#include "CallStack.h"

CallStack::CallStack () { }
CallStack::CallStack(
    unsigned long int capacity
) {
    this->capacity = capacity;
    if(this->capacity <= 0) {
        this->capacity = 4294967295;
    }
}

void CallStack::pushback(std::vector<Executable>* items) {
    if (this->size() >= this->capacity) {
        Serial.println("Stack has reached max capacity.");
    }
    for ( auto &item : *items ) {
        this->storage.push_back(&item);
    }
}
void CallStack::pushinfrontof(Executable* infrontof, std::vector<Executable>* items) {
    if (this->size() >= this->capacity) {
        Serial.println("Stack has reached max capacity.");
    }
    std::list<Executable*>::iterator it;

    it = this->storage.begin();
    for(auto executable : this->storage) {
        if(executable == infrontof) {
            break;
        } else {
            ++it; 
        }
    }

    for ( auto &item : *items ) {
        this->storage.insert(it, &item);
    }
}

void CallStack::pushfront(std::vector<Executable>* items) {
    if (this->size() >= this->capacity) {
        Serial.println("Stack has reached max capacity.");
    }
    std::list<Executable*>::iterator it;
    it = this->storage.begin();
    for ( auto &item : *items ) {
        this->storage.insert(it, &item);
    }
}
void CallStack::delayScheduledSection(Executable* parent) {
    this->storage.remove(parent);
    std::vector<Executable> array = {*parent};
    this->pushback(&array);
    this->delayed++;
    if(!!parent->parentAwait) {
        this->delayScheduledSection(parent->parentAwait);
    }
}
bool CallStack::doProcessMore() {
    return this->delayed < this->size();
}
void CallStack::prepareStep() {
    this->delayed = 0;
}
Executable* CallStack::pop() {
    Executable* front = this->storage.front();
    this->storage.pop_front();
    return front;
}
unsigned long int CallStack::size() {
    return this->storage.size();
}