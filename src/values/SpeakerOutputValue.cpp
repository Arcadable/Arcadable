#include "SpeakerOutputValue.h"
#include "../configuration.h"
#include "../gameState.h"

SpeakerOutputValue::SpeakerOutputValue(
    unsigned short ID,
    unsigned short index,
    SoundController *soundController
) : Value(ID, ValueType::speakerOutputPointer) {
    this->index = index;
    this->isNumberType = true;
    this->soundController = soundController;
}
SpeakerOutputValue::SpeakerOutputValue() {}
void SpeakerOutputValue::init(std::vector<unsigned short> ids) {}

double SpeakerOutputValue::getNumber() {
    return SPEAKER_OUTPUT_PINS[this->index];
}
void SpeakerOutputValue::setNumber(double newValue) {
    this->soundController->playTone(newValue, 1.0, SPEAKER_OUTPUT_PINS[this->index], 1000);

}
bool SpeakerOutputValue::isTruthy() {
    return true;
}

std::vector<unsigned short>* SpeakerOutputValue::getValueArray() { return &this->emptyArray; };
void SpeakerOutputValue::setValueArray(std::vector<unsigned short> newValue) { };