#ifndef _ARC_SOUND
#define _ARC_SOUND
#include <Arduino.h>
#include "configuration.h"
#include "i2cController.h"

class SoundController {
  public: 
    uint8_t buff[512] = {};
    uint8_t buffIndex = 0; 

    I2cController *i2cController;
    SoundController() {

    }
    void init(I2cController *i2cController) {
      this->i2cController = i2cController;
      this->buffIndex = 0;
    }

    void playTone(double freq, double vol, unsigned int pin, unsigned long duration) {
/*
      unsigned long roundedFreq = (unsigned long)(freq * 10000);
      this->buff[0+buffIndex]=  roundedFreq & 0x000000ff;
      this->buff[1+buffIndex]= (roundedFreq & 0x0000ff00) >> 8;
      this->buff[2+buffIndex]= (roundedFreq & 0x00ff0000) >> 16;
      this->buff[3+buffIndex]= (roundedFreq & 0xff000000) >> 24;

      unsigned long roundedVol = (unsigned long)(vol * 1000000000);
      this->buff[4+buffIndex]=  roundedVol & 0x000000ff;
      this->buff[5+buffIndex]= (roundedVol & 0x0000ff00) >> 8;
      this->buff[6+buffIndex]= (roundedVol & 0x00ff0000) >> 16;
      this->buff[7+buffIndex]= (roundedVol & 0xff000000) >> 24;

      this->buff[8+buffIndex]=   pin & 0x000000ff;
      this->buff[9+buffIndex]=  (pin & 0x0000ff00) >> 8;
      this->buff[10+buffIndex]= (pin & 0x00ff0000) >> 16;
      this->buff[11+buffIndex]= (pin & 0xff000000) >> 24;

      this->buff[12+buffIndex]=  duration & 0x000000ff;
      this->buff[13+buffIndex]= (duration & 0x0000ff00) >> 8;
      this->buff[14+buffIndex]= (duration & 0x00ff0000) >> 16;
      this->buff[15+buffIndex]= (duration & 0xff000000) >> 24;
      this->buffIndex += 16;*/
    }
    
    void sendSounds() {
      if(this->buffIndex > 0) {
        this->i2cController->write(SOUND_CONTROLLER_ADDRESS, this->buffIndex, this->buff);
        this->buffIndex = 0;
      }
    }


};

#endif