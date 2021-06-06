#ifndef _ARC_I2C
#define _ARC_I2C

#include <Arduino.h>
#include "../configuration.h"
#include <i2c_device.h>
#include "imx_rt1060/imx_rt1060_i2c_driver.h"

class I2cController {
  public: 

    static I2CMaster& master;

    static unsigned long noError;
    static unsigned long error0;
    static unsigned long error1;
    static unsigned long error2;
    static unsigned long error3;
    static unsigned long error4;
    static unsigned long error5;
    static unsigned long error6;
    static unsigned long error7;
    static unsigned long error8;
    static unsigned long error9;
    static unsigned long error10;
    static unsigned long error11;
    static unsigned long error12;

    I2cController() {

    }
    static void init() {
      master.begin(100'000);
      pinMode(STUCK_BUS_PIN_MAIN_CONTROLLER, OUTPUT);
      digitalWrite(STUCK_BUS_PIN_MAIN_CONTROLLER, false);
    }

    static bool isAvailable(unsigned char hardwareAddress) {
      uint8_t buffer[] = {};
      master.write_async(hardwareAddress, buffer, 0, true);
      finish();

      I2CError status = master.error();
      if (status == I2CError::ok) {
          return true;
      } else if (status == I2CError::address_nak) {
          return false;
      } else {
        if(I2C_DEBUG) {
          Serial.print("Unexpected error at address ");
          Serial.println(hardwareAddress);
        }
        return false;
      }
    }
    


    static bool write(unsigned char hardwareAddress, unsigned int dataLength, unsigned char *data) {
      master.write_async(hardwareAddress, data, dataLength, true);
      finish();
      return true;
    }

    static void finish() {
      elapsedMillis timeout;
      bool finished = false;
      while (timeout < 200) {
          if (master.finished()){
            finished = true;
            break;
          }
      }
      if(!finished) {
        if(I2C_DEBUG) {
          Serial.println("Master: ERROR timed out waiting for transfer to finish.");
        }
        digitalWriteFast(STUCK_BUS_PIN_MAIN_CONTROLLER, true);
      }
      if (master.has_error()) {
        switch((unsigned int)master.error()) {
          case 0: {
            error0++;
            break;
          }
          case 1: {
            error1++;
            break;
          }
          case 2: {
            error2++;
            break;
          }
          case 3: {
            error3++;
            break;
          }
          case 4: {
            error4++;
            break;
          }
          case 5: {
            error5++;
            break;
          }
          case 6: {
            error6++;
            break;
          }
          case 7: {
            error7++;
            break;
          }
          case 8: {
            error8++;
            break;
          }
          case 9: {
            error9++;
            break;
          }
          case 10: {
            error10++;
            break;
          }
          case 11: {
            error11++;
            break;
          }
          case 12: {
            error12++;
            digitalWriteFast(STUCK_BUS_PIN_MAIN_CONTROLLER, true);
            break;
          }
        }
      } else {
        noError++;
        digitalWriteFast(STUCK_BUS_PIN_MAIN_CONTROLLER, false);
      }
      if(I2C_DEBUG) {
        Serial.print("noError: "); Serial.println(noError);
        Serial.print("error0: "); Serial.println(error0);
        Serial.print("error1: "); Serial.println(error1);
        Serial.print("error2: "); Serial.println(error2);
        Serial.print("error3: "); Serial.println(error3);
        Serial.print("error4: "); Serial.println(error4);
        Serial.print("error5: "); Serial.println(error5);
        Serial.print("error6: "); Serial.println(error6);
        Serial.print("error7: "); Serial.println(error7);
        Serial.print("error8: "); Serial.println(error8);
        Serial.print("error9: "); Serial.println(error9);
        Serial.print("error10: "); Serial.println(error10);
        Serial.print("error11: "); Serial.println(error11);
        Serial.print("error12: "); Serial.println(error12);
      }

    }

    static bool read(unsigned char hardwareAddress, unsigned int startAddress, unsigned int readLength, unsigned char *dataReceiver) {
 
      unsigned char startAddressData[2] = {static_cast<unsigned char>(startAddress >> 8), static_cast<unsigned char>(startAddress & 0xFF)};
      write(hardwareAddress, 2, startAddressData);
      master.read_async(hardwareAddress, dataReceiver, readLength, true);
      finish();

     /* unsigned char data[300] = {0,4,11,0,0,1,0,36,8,0,7,0,0,5,0,6,0,10,0,0,1,0,2,0,12,26,0,1,0,11,0,14,28,0,1,0,13,0,15,26,0,4,0,7,0,85,0,0,1,0,0,0,0,0,2,61,204,204,205,0,3,63,128,0,0,0,5,0,0,0,0,0,6,64,0,0,0,0,8,189,204,204,205,0,9,61,204,204,205,0,11,65,160,0,0,0,13,0,0,0,0,0,16,68,250,0,0,0,17,65,112,0,0,0,18,66,200,0,0,0,19,68,122,0,0,0,20,68,187,128,0,0,4,4,0,4,2,0,3,139,0,8,0,11,134,0,9,0,16,0,1,0,17,0,17,0,25,128,0,1,0,2,0,8,0,2,0,2,0,9,0,3,0,5,0,4,0,4,0,1,0,10,0,25,129,0,5,0,12,255,255,0,1,0,6,0,14,255,255,0,2,0,7,0,15,255,255,0,3,0,5,141,0,0,0,6,0,5,147,0,12,0,6,0,5,145,0,11,0,20,0,11,146,0,10,0,0,0,3,0,18,0,19,0,55,143,0,0,0,1,0,0,0,4,0,1,0,7,0,5,0,2,0,8,0,9,0,1,0,1,0,1,0,2,0,1,0,2,0,3,0,4,0,3,0,4,0,5,0,6,0,6,128,3,0,10,0,11,0,12};
      std::copy(data + startAddress,
                data + startAddress + readLength,
                dataReceiver + 0);*/

      return true;
    }
};

#endif