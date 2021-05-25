#include "ToneInstruction.h"
#include "Arduino.h"
#include "../executable.h"
#include "../gameState.h"
ToneInstruction::ToneInstruction (
    unsigned short ID,
    bool await,
    GameState *game,
    SoundController *soundController
) : Instruction(ID, InstructionType::Tone, await) {
    this->game = game;
    this->soundController = soundController;
}
ToneInstruction::ToneInstruction() {}

void ToneInstruction::init(std::vector<unsigned short> ids) {
    this->speakerOutputValue = this->game->values.find(ids[0])->second;
    this->volumeValue = this->game->values.find(ids[1])->second;
    this->frequencyValue = this->game->values.find(ids[2])->second;
    this->durationValue = this->game->values.find(ids[3])->second;

}
std::vector<unsigned int>* ToneInstruction::action(bool async) {

    unsigned long speakerPin = static_cast<unsigned long>(this->speakerOutputValue->getNumber());
    double volume = this->volumeValue->getNumber();
    double frequency = this->frequencyValue->getNumber();
    unsigned long duration = static_cast<unsigned long>(this->durationValue->getNumber());


    if(volume > 1) {
        volume = 1;
    } else if( volume < 0) {
        volume = 0;
    }
    if( frequency < 1) {
        frequency = 1;
    }
  
    this->soundController->playTone(frequency, volume, speakerPin, duration);

    return &Executable::empty;
}
double ToneInstruction::getWaitAmount() {
    return this->durationValue->getNumber();
}
