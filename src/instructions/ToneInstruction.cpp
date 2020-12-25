#include "WaitInstruction.h"
#include "Arduino.h"
#include "values/ListValue.h"
#include "Arcadable.h"
ToneInstruction::ToneInstruction (
    unsigned short ID,
    bool await
) : Instruction(ID, InstructionType::Tone, await) {}
ToneInstruction::ToneInstruction() {}

void ToneInstruction::init(std::vector<unsigned short> ids) {
    this->speakerOutputValue = Arcadable::getInstance()->values.find(ids[0])->second;
    this->volumeValue = Arcadable::getInstance()->values.find(ids[1])->second;
    this->frequencyValue = Arcadable::getInstance()->values.find(ids[2])->second;
    this->durationValue = Arcadable::getInstance()->values.find(ids[3])->second;
    
    this->executables = {Executable([this] () -> const std::vector<Executable>& {
        int speakerPin = static_cast<int>(this->speakerOutputValue->getNumber());
        double volume = this->volumeValue->getNumber();
        double frequency = this->frequencyValue->getNumber();
        double duration = this->durationValue->getNumber();

        if(volume > 1) {
            volume = 1;
        } else if( volume < 0) {
            volume = 0;
        }
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
        toneOnTimer.begin([speakerPin, volume] {
            analogWrite(speakerPin, 1023 * volume);
        }, waveLength);

        halfTimer.trigger(waveLength / 2);
        stopTimer.trigger(duration * 1000);

        return this->empty;


    }, false, false, NULL, NULL)};
}
