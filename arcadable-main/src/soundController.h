#ifndef _ARC_SOUND
#define _ARC_SOUND
#include <Arduino.h>
#include "../configuration.h"
#include "i2cController.h"

class SoundController {
  public: 
    static unsigned char buffer[256];
    static unsigned char bufferIndex;

    SoundController() {

    }

    static void playTone(double freq, double vol, unsigned int pin, unsigned long duration) {
      unsigned long roundedFreq = (unsigned long)(freq * 10000);

      SoundController::buffer[SoundController::bufferIndex + 0] = roundedFreq & 0x000000ff;
      SoundController::buffer[SoundController::bufferIndex + 1] = (roundedFreq & 0x0000ff00) >> 8;
      SoundController::buffer[SoundController::bufferIndex + 2] = (roundedFreq & 0x00ff0000) >> 16;
      SoundController::buffer[SoundController::bufferIndex + 3] = (roundedFreq & 0xff000000) >> 24;

      unsigned long roundedVol = (unsigned long)(vol * 1000000000);

      SoundController::buffer[SoundController::bufferIndex + 4]= roundedVol & 0x000000ff;
      SoundController::buffer[SoundController::bufferIndex + 5]= (roundedVol & 0x0000ff00) >> 8;
      SoundController::buffer[SoundController::bufferIndex + 6]= (roundedVol & 0x00ff0000) >> 16;
      SoundController::buffer[SoundController::bufferIndex + 7]= (roundedVol & 0xff000000) >> 24;
      
      SoundController::buffer[SoundController::bufferIndex + 8]= pin & 0x000000ff;
      SoundController::buffer[SoundController::bufferIndex + 9]= (pin & 0x0000ff00) >> 8;
      SoundController::buffer[SoundController::bufferIndex + 10]= (pin & 0x00ff0000) >> 16;
      SoundController::buffer[SoundController::bufferIndex + 11]= (pin & 0xff000000) >> 24;

      SoundController::buffer[SoundController::bufferIndex + 12]= duration & 0x000000ff;
      SoundController::buffer[SoundController::bufferIndex + 13]= (duration & 0x0000ff00) >> 8;
      SoundController::buffer[SoundController::bufferIndex + 14]= (duration & 0x00ff0000) >> 16;
      SoundController::buffer[SoundController::bufferIndex + 15]= (duration & 0xff000000) >> 24;

      SoundController::bufferIndex += 16;


    }
    
    static void sendSounds() {

      if(SoundController::bufferIndex > 0) {
        I2cController::write(SOUND_CONTROLLER_ADDRESS, SoundController::bufferIndex, SoundController::buffer);
        SoundController::bufferIndex = 0;
      }
    }


};

#endif