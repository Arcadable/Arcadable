#pragma once
#define ToneInstruction_h
#include "Instruction.h"
#include "../values/Value.h"
#include <vector>
#include "../soundController.h"
class GameState;

class ToneInstruction: public Instruction {
	public:
        GameState *game;
        SoundController *soundController;
        Value* speakerOutputValue;
        Value* volumeValue;
        Value* frequencyValue;
        Value* durationValue;

        ToneInstruction();
        ToneInstruction(
            unsigned short ID,
            bool await,
            GameState *game,
            SoundController *soundController
        );
        void init(std::vector<unsigned short> ids);
        std::vector<unsigned int>* action(bool async);
        double getWaitAmount();
        

};
