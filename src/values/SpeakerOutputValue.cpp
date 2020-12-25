#include "SpeakerOutputValue.h"
#include <Arcadable.h>

SpeakerOutputValue::SpeakerOutputValue(
    unsigned short ID,
    unsigned short index
) : Value(ID, ValueType::speakerOutputPointer) {
    this->index = index;
    this->isNumberType = true;
}
SpeakerOutputValue::SpeakerOutputValue() {}
void SpeakerOutputValue::init(std::vector<unsigned short> ids) {}

double SpeakerOutputValue::getNumber() {
    return Arcadable::getInstance()->systemConfig->speakerOutputPins->at(this->index);
}
void SpeakerOutputValue::setNumber(double newValue) {

    int speakerPin = index;
    double frequency = newValue;

    if( frequency < 1) {
        frequency = 1;
    }

    double waveLength = 1000'000 / frequency;

    PeriodicTimer toneOnTimer(TCK);
    PeriodicTimer toneOffTimer(TCK);
    OneShotTimer  halfTimer(TCK);
    OneShotTimer  stopTimer(TCK);

    stopTimer.begin([&toneOffTimer, &toneOnTimer, speakerPin] {
        toneOffTimer.stop();
        toneOnTimer.stop();
        analogWrite(speakerPin, 0);
    });

    halfTimer.begin([&toneOffTimer, speakerPin, waveLength] {
        toneOffTimer.begin([speakerPin] {
            analogWrite(speakerPin, 0);
        }, waveLength);
    });
    toneOnTimer.begin([speakerPin] {
        analogWrite(speakerPin, 1023);
    }, waveLength);

    halfTimer.trigger(waveLength / 2);
    stopTimer.trigger(1000'000);
}
bool SpeakerOutputValue::isTruthy() {
    return true;
}

std::vector<unsigned short>* SpeakerOutputValue::getValueArray() { return &this->emptyArray; };
void SpeakerOutputValue::setValueArray(std::vector<unsigned short> newValue) { };