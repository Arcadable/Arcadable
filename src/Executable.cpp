#include "Executable.h"

Executable::Executable() {}
Executable::Executable(
    std::function<const std::vector<Executable>& ()> action,
    bool async,
    bool await,
    Executable* parentAwait,
    std::function<unsigned long int ()> waitMillis
) {
    this->action = action;
    this->async = async;
    this->await = await;
    this->awaiting = {};
    this->parentAwait = parentAwait;
    this->waitMillis = waitMillis;
}
void Executable::checkWaitMillis() {
    if(this->waitMillis != NULL) {
        unsigned long int extraMillis = this->waitMillis();
        if(extraMillis > 0) {
            this->executeOnMillis = millis() + extraMillis;
        } else {
            this->executeOnMillis = 0;
        }
        this->waitMillis == NULL;
    }
};
Executable* Executable::withParentAwait(Executable* p) {
    this->parentAwait = p;
    return this;
}