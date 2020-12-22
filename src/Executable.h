#pragma once
#define Executable_h
#include <vector>
#include <functional>
#include "Arduino.h"

class Executable {
    public:
        std::function<const std::vector<Executable>& ()> action;
        bool async;
        bool await;
        std::vector<Executable>& awaiting;
        Executable* parentAwait;
        std::function<unsigned long int ()> waitMillis;

        Executable();
        Executable(
            std::function<const std::vector<Executable>& ()> action,
            bool async,
            bool await,
            std::vector<Executable>& awaiting,
            Executable* parentAwait,
            std::function<unsigned long int ()> waitMillis
        );

        void checkWaitMillis();

        Executable* withParentAwait(Executable* p);
};
