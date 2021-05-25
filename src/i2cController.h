#ifndef _ARC_I2C
#define _ARC_I2C

#include <Arduino.h>
#include "configuration.h"
#include <i2c_device.h>
#include "imx_rt1060/imx_rt1060_i2c_driver.h"

class I2cController {
  public: 

    I2CMaster& master = Master;

    unsigned long noError = 0;
    unsigned long error0 = 0;
    unsigned long error1 = 0;
    unsigned long error2 = 0;
    unsigned long error3 = 0;
    unsigned long error4 = 0;
    unsigned long error5 = 0;
    unsigned long error6 = 0;
    unsigned long error7 = 0;
    unsigned long error8 = 0;
    unsigned long error9 = 0;
    unsigned long error10 = 0;
    unsigned long error11 = 0;
    unsigned long error12 = 0;

    uint8_t stuck_bus_pin = 12;

    I2cController() {

    }
    void init() {
      master.begin(100'000);
      pinMode(stuck_bus_pin, OUTPUT);
      digitalWrite(stuck_bus_pin, false);
    }

    bool isAvailable(unsigned char hardwareAddress) {
      
      uint8_t buffer[] = {};
      master.write_async(hardwareAddress, buffer, 0, true);
      finish();

      I2CError status = master.error();
      if (status == I2CError::ok) {
          return true;
      } else if (status == I2CError::address_nak) {
          return false;
      } else {
          Serial.print("Unexpected error at address ");
          Serial.println(hardwareAddress);
          return false;
      }
    }
    


    bool write(unsigned char hardwareAddress, unsigned int dataLength, unsigned char *data) {
      master.write_async(hardwareAddress, data, dataLength, true);
      finish();
      return true;
    }

    void finish() {
      elapsedMillis timeout;
      bool finished = false;
      while (timeout < 200) {
          if (master.finished()){
            finished = true;
            break;
          }
      }
      if(!finished) {
        Serial.println("Master: ERROR timed out waiting for transfer to finish.");
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
            digitalWriteFast(stuck_bus_pin, true);
            break;
          }
        }
      } else {
        noError++;
        digitalWriteFast(stuck_bus_pin, false);
      }
      /*Serial.print("noError: "); Serial.println(noError);
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
      Serial.print("error12: "); Serial.println(error12);*/
    }

    bool read(unsigned char hardwareAddress, unsigned int startAddress, unsigned int readLength, unsigned char *dataReceiver) {
     // unsigned char data[246] = {0,4,11,0,0,1,0,22,8,0,6,0,0,1,0,2,0,8,26,0,1,0,7,0,10,28,0,1,0,9,0,73,0,0,1,0,0,0,0,0,2,61,204,204,205,0,3,63,128,0,0,0,4,189,204,204,205,0,5,61,204,204,205,0,7,65,160,0,0,0,9,0,0,0,0,0,11,68,250,0,0,0,12,65,112,0,0,0,13,66,200,0,0,0,14,68,122,0,0,0,15,68,187,128,0,0,3,139,0,6,0,11,134,0,7,0,11,0,1,0,12,0,12,0,19,128,0,1,0,2,0,4,0,2,0,2,0,5,0,3,0,1,0,6,0,17,129,0,4,0,8,255,255,0,1,0,5,0,10,255,255,0,2,0,5,141,0,0,0,5,0,5,147,0,10,0,5,0,5,145,0,9,0,15,0,11,146,0,8,0,0,0,3,0,13,0,14,0,47,143,0,0,0,1,0,0,0,3,0,3,0,3,0,4,0,5,0,4,0,2,0,6,0,7,0,1,0,1,0,1,0,2,0,1,0,2,0,5,128,3,0,8,0,9,0,10};
      unsigned char startAddressData[2] = {static_cast<unsigned char>(startAddress >> 8), static_cast<unsigned char>(startAddress & 0xFF)};
      this->write(hardwareAddress, 2, startAddressData);
      master.read_async(hardwareAddress, dataReceiver, readLength, true);
      finish();
     /* std::copy(data + startAddress,
                data + startAddress + readLength,
                dataReceiver + 0);*/

      return true;
    }
};

#endif