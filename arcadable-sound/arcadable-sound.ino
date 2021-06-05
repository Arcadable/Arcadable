#include <Arduino.h>
#include <i2c_driver.h>
#include "imx_rt1060/imx_rt1060_i2c_driver.h"
#include "vector"
#include "TeensyTimerTool.h"
#include <map>
#include "algorithm"
#include <math.h> 
#include "configuration.h"

using namespace TeensyTimerTool;
using namespace std;

double waveStepMicroseconds = 50;
double waveStepSeconds = waveStepMicroseconds / 1000000;
double analogRange = 255;
unsigned int analogResolution = 8;
unsigned int analogFrequency = 585937.5;
double generalVolume = 0.2;
unsigned int restAnalog = analogRange * generalVolume / 2;

struct Sound {      
  unsigned int speaker;
  unsigned int waveStepsStop;
  unsigned long waveStepsSinceStart;
  unsigned int waveStepLength;
  std::map<unsigned int, unsigned int> sine;
  Sound(double f, double v, int s, double d) {   
    speaker = s;

    double analogVolume = analogRange * v * generalVolume;
    double waveStepFrequency = f * waveStepSeconds;
    double waveStepFrequencyCalc = waveStepFrequency * 6.283;
    double t = (d / 1000) / waveStepSeconds;
    waveStepLength = 1/waveStepFrequency;
    waveStepsStop = (floor(t/waveStepLength)+0.5)*waveStepLength;
    waveStepsSinceStart = waveStepLength * 0.75;

    for (double i = 0; i <= waveStepLength; i++) {
      sine[(unsigned int)i] = (unsigned int)((sin(waveStepFrequencyCalc * i) + 1.0) / 2.0 * analogVolume);
    }

  }

  bool operator==(const Sound& a) const {
    return (
      speaker == a.speaker &&
      waveStepsStop == a.waveStepsStop &&
      waveStepsSinceStart == a.waveStepsSinceStart &&
      waveStepLength == a.waveStepLength
    );
  }
   
};    
std::vector<Sound> sounds;
PeriodicTimer waveTimer = PeriodicTimer(TCK);

I2CSlave& slave = Slave;
const size_t slave_rx_buffer_size = 512;
uint8_t slave_rx_buffer[slave_rx_buffer_size] = {};
uint8_t slave_rx_buffer_2[slave_rx_buffer_size] = {};
volatile size_t slave_bytes_received = 0;
void after_receive(size_t, uint16_t);

boolean isStuck = false;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Started listening");
  Serial.println(SOUND_CONTROLLER_ADDRESS);
  slave.after_receive(after_receive);
  slave.set_receive_buffer(slave_rx_buffer, slave_rx_buffer_size);
  slave.listen(SOUND_CONTROLLER_ADDRESS);

  attachInterrupt(digitalPinToInterrupt(STUCK_BUS_PIN_SOUND_CONTROLLER), bus_stuck, RISING);


  analogWriteResolution(analogResolution);

  for(unsigned char j = 0; j < SPEAKER_OUTPUT_PINS_AMOUNT; j++) {
    pinMode(SPEAKER_OUTPUT_PINS[j], OUTPUT);
  }
  waveTimer.begin(_wave, waveStepMicroseconds);
}

void bus_stuck() {
  isStuck = true;
}

void _wave() {

  for(auto& sound: sounds) {
    sound.waveStepsSinceStart++;
    if(sound.waveStepsStop > sound.waveStepsSinceStart) {
      analogWrite(sound.speaker, sound.sine[sound.waveStepsSinceStart%sound.waveStepLength]);
    } else {
      analogWrite(sound.speaker, 0);
      sounds.erase(std::remove(sounds.begin(), sounds.end(), sound), sounds.end());
    }
  }
}

void loop() {
  if(millis() % 1000 == 0) {
    Serial.println("ALIVE!");
    delayMicroseconds(500);
  }

  if(slave_bytes_received > 0) {
    for(unsigned int i = 0; i < slave_bytes_received; i += 16) {
      double freq = (slave_rx_buffer_2[i + 0] & 0x000000ff) +
        ((slave_rx_buffer_2[i + 1] & 0x000000ff) << 8) +
        ((slave_rx_buffer_2[i + 2] & 0x000000ff) << 16) +
        ((slave_rx_buffer_2[i + 3] & 0x000000ff) << 24);
      
      double vol = ((double)((slave_rx_buffer_2[i + 4] & 0x000000ff) +
        ((slave_rx_buffer_2[i + 5] & 0x000000ff) << 8) +
        ((slave_rx_buffer_2[i + 6] & 0x000000ff) << 16) +
        ((slave_rx_buffer_2[i + 7] & 0x000000ff) << 24)));
        
      unsigned int pin = (slave_rx_buffer_2[i + 8] & 0x000000ff) +
        ((slave_rx_buffer_2[i + 9] & 0x000000ff) << 8) +
        ((slave_rx_buffer_2[i + 10] & 0x000000ff) << 16) +
        ((slave_rx_buffer_2[i + 11] & 0x000000ff) << 24);
        
      double duration = (slave_rx_buffer_2[i + 12] & 0x000000ff) +
        ((slave_rx_buffer_2[i + 13] & 0x000000ff) << 8) +
        ((slave_rx_buffer_2[i + 14] & 0x000000ff) << 16) +
        ((slave_rx_buffer_2[i + 15] & 0x000000ff) << 24);
      sounds.push_back(Sound(freq/10000.0, vol/1000000000.0, pin, duration)); 
      Serial.print(freq/10000);Serial.print("-");
      Serial.print(vol/1000000000);Serial.print("-");
      Serial.print(pin);Serial.print("-");
      Serial.println(duration);
    }
    slave_bytes_received = 0;

    if (slave.has_error()) {

        switch((int)slave.error()) {
            case 0: {
                //error0++;
                break;
            }
            case 1: {
                //error1++;
                break;
            }
            case 2: {
                //error2++;
                break;
            }
            case 3: {
                //error3++;
                slave.listen(SOUND_CONTROLLER_ADDRESS);
                break;
            }
            case 4: {
                //error4++;
                break;
            }
            case 5: {
                //error5++;
                break;
            }
            case 6: {
                //error6++;
                break;
            }
            case 7: {
                //error7++;
                break;
            }
            case 8: {
                //error8++;
                break;
            }
            case 9: {
                //error9++;
                break;
            }
            case 10: {
                //error10++;
                break;
            }
            case 11: {
                //error11++;
                break;
            }
            case 12: {
                //error12++;
                break;
            }
        }
    } else {
        //noError++;
    }
  }
  if(isStuck) {
    //stuck++;
    slave.listen(SOUND_CONTROLLER_ADDRESS);
    isStuck = false;
  }

}

void after_receive(size_t length, uint16_t address) {
  if(length % 16 == 0) {
    memcpy(slave_rx_buffer_2, slave_rx_buffer, length);
    slave_bytes_received = length;
  }
} 
