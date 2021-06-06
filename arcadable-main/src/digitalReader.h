#ifndef _ARC_DIGITAL_READER
#define _ARC_DIGITAL_READER
#include <map>
#include <Arduino.h>
#include "../configuration.h"

class DigitalReader {
  public: 
    std::map<unsigned char, bool> digitalInputValues;

    DigitalReader() {

    }
    void init() {
      pinMode(REG_CLOCK_INHIBIT_PIN, OUTPUT);
      pinMode(REG_SHIFT_LOAD_PIN, OUTPUT);
      pinMode(REG_SERIAL_OUTPUT_PIN, INPUT);
      pinMode(REG_CLOCK_INPUT_PIN, OUTPUT);

      for(unsigned char i = 0; i < REG_DATA_WIDTH ; i++) {
        digitalInputValues.insert(std::pair<unsigned char, bool>(i, false)); 

      }
    }
    void updateValues() {
      digitalWriteFast(REG_CLOCK_INHIBIT_PIN, HIGH);
      digitalWriteFast(REG_SHIFT_LOAD_PIN, LOW);
      digitalWriteFast(REG_SHIFT_LOAD_PIN, HIGH);
      digitalWriteFast(REG_CLOCK_INHIBIT_PIN, LOW);
      if(LOG_INPUTS) {
        Serial.println();
        Serial.println("DIGITAL INPUTS");
      }
      for(unsigned char i = 0; i < REG_DATA_WIDTH ; i++) {
        digitalInputValues[i] = digitalReadFast(REG_SERIAL_OUTPUT_PIN);
        digitalWriteFast(REG_CLOCK_INPUT_PIN, HIGH);
        digitalWriteFast(REG_CLOCK_INPUT_PIN, LOW);
        if(LOG_INPUTS) {
          if(i < 10) {
            Serial.print("0"); 
          }
          Serial.print(i); Serial.print(": "); Serial.print(digitalInputValues[i]); Serial.print(", "); 
          if(i % 4 == 3) {
            Serial.println();
          }
        }
      }
    }



};

#endif