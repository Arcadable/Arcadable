#pragma once
#define SpeakerOutputValue_h
#include "Value.h"
#include <vector>
#include "../soundController.h"
class GameState;
class SpeakerOutputValue: public Value {
	public:
        unsigned short index;

        double getNumber();
        void setNumber(double newValue);
        std::vector<unsigned short>* getValueArray();
        void setValueArray(std::vector<unsigned short> newValue);

        void init(std::vector<unsigned short> ids);
		bool isTruthy();
        SpeakerOutputValue();
        SpeakerOutputValue(
            unsigned short ID,
            unsigned short index
        );
};
