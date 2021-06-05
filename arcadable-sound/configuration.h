#ifndef ARC_CONFIG
#define ARC_CONFIG

#ifndef ARDUINO_TEENSY40
    #define ARDUINO_TEENSY40
#endif
#ifndef TEENSYDUINO
    #define TEENSYDUINO
#endif

// Speaker output
#define SPEAKER_OUTPUT_PINS_AMOUNT 4
#define SPEAKER_OUTPUT_PINS (int[]){0,1,2,3}
#define GENERAL_VOLUME 0.02

//I2C
#define STUCK_BUS_PIN_SOUND_CONTROLLER 13
#define SOUND_CONTROLLER_ADDRESS (unsigned char)0b1010001

#endif