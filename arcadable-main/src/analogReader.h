#ifndef _ARC_ANALOG_READER
#define _ARC_ANALOG_READER
#include <map>
#include <Arduino.h>
#include "../configuration.h"
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

class AnalogReader {
  public: 
    std::map<unsigned char, unsigned short int> analogInputValues;

    AnalogReader() {

    }
    void init() {
      for(unsigned char j = 0; j < ANALOG_SIGNAL_PINS_AMOUNT; j++) {
        pinMode(ANALOG_SIGNAL_PINS[j], OUTPUT);
      }
      for(unsigned char i = 0; i < ANALOG_INPUTS_AMOUNT ; i++) {
        analogInputValues.insert(std::pair<unsigned char, unsigned short int>(i, 512)); 
      }
    }
    void updateValues() {
      if(LOG_INPUTS) {
        Serial.println();
        Serial.println("ANALOG INPUTS");
      }
      for(unsigned char i = 0; i < ANALOG_INPUTS_AMOUNT ; i++) {
        unsigned char index = 0;
        for(unsigned char j = 0; j < ANALOG_SIGNAL_PINS_AMOUNT; j++) {
            digitalWrite(ANALOG_SIGNAL_PINS[j], (i >> index) & 0b1);
            index++;
        }
        unsigned short readValue = analogRead(ANALOG_INPUT_PIN);
        analogInputValues[i] = readValue;

        if(LOG_INPUTS) {
          if(analogInputValues[i] < 1000) {
            Serial.print("0"); 
          }
          if(analogInputValues[i] < 100) {
            Serial.print("0"); 
          }
          if(analogInputValues[i] < 10) {
            Serial.print("0"); 
          }
          Serial.print(analogInputValues[i]); Serial.print(", "); 
          if(i % 2 == 1) {
            Serial.println();
          }
        }
      }
    }



};

#endif