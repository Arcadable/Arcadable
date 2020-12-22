#include "CallStack.h"


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
        this->storage.push_back(item);
    }
}
void CallStack::pushinfrontof(Executable* infrontof, std::vector<Executable>* items) {
    if (this->size() >= this->capacity) {
        Serial.println("Stack has reached max capacity.");
    }
    for ( auto &item : *items ) {
        this->storage.insert(infrontof, item);
    }
}

void CallStack::pushfront(std::vector<Executable>* items) {
    if (this->size() >= this->capacity) {
        Serial.println("Stack has reached max capacity.");
    }
    Executable* front = this->storage.front();
    for ( auto &item : *items ) {
        this->storage.insert(front, item);
    }
}
void CallStack::delayScheduledSection(Executable* parent) {
    this->storage.erase(parent);
    this->push_back(parent);
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