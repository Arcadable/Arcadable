#ifndef ARC_CONFIG
#define ARC_CONFIG

#ifndef ARDUINO_TEENSY40
    #define ARDUINO_TEENSY40
#endif
#ifndef TEENSYDUINO
    #define TEENSYDUINO
#endif

// Display
#define NUM_LEDS_PER_SECTION 252
#define NUM_SECTIONS 7
#define NUM_LEDS_TOTAL NUM_LEDS_PER_SECTION * NUM_SECTIONS
#define DATA_PINS (int[]){29, 14, 24, 1, 8, 20, 17}
#define MAX_BRIGHTNESS 250
#define MAX_MILLIAMPS 90000
#define SCREEN_WIDTH 42
#define SCREEN_HEIGHT 42
#define LAYOUT_IS_ZIG_ZAG true

// Speaker output
#define SPEAKER_OUTPUT_PINS_AMOUNT 4
#define SPEAKER_OUTPUT_PINS (int[]){7,4,3,5}
#define GENERAL_VOLUME 0.02

// Inputs 
#define LOG_INPUTS true
#define REG_CLOCK_INHIBIT_PIN 4
#define REG_SHIFT_LOAD_PIN 6
#define REG_SERIAL_OUTPUT_PIN 7
#define REG_DATA_WIDTH 16
#define REG_CLOCK_INPUT_PIN 5

#define ANALOG_INPUTS_AMOUNT 8
#define ANALOG_INPUT_PIN A1
#define ANALOG_SIGNAL_PINS_AMOUNT 4
#define ANALOG_SIGNAL_PINS (int[]){9, 16, 10, 11}

// Gamecard
#define GAMECARD_POLLING_INTERVAL (unsigned short int)1000
#define GAMECARD_EEPROM_ADDRESS (unsigned char)0b1010000
#define GAMECARD_READ_WRITE_BUFFER_SIZE (unsigned char)32

// Sound communication
#define SOUND_CONTROLLER_ADDRESS (unsigned char)0b1010001

#endif