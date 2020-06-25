#pragma once
#define ValuePointer_h
#include <Arcadable.h>

template <typename T> 
class ValuePointer {
	public:
		unsigned short ID;
		Arcadable *game;
		virtual T getObject();
        virtual template <typename T2> T2 getValue();

        ValuePointer();
        ValuePointer(
            unsigned short ID
        );
};
