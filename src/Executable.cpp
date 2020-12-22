#include "Executable.h"

void Executable::checkWaitMillis() {

};
Executable* Executable::withParentAwait(Executable* p) {

}
Executable::Executable(
    std::function<const std::vector<Executable>& ()> action,
    bool async,
    bool await,
    std::vector<Executable>& awaiting,
    Executable* parentAwait,
    std::function<unsigned long int ()> waitMillis
): awaiting(awaiting) {
    this->action = action;
    this->async = async;
    this->await = await;
    this->parentAwait = parentAwait;
    this->waitMillis = waitMillis;
}
