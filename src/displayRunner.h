#ifndef _ARC_DISPLAY
#define _ARC_DISPLAY

#include <Arduino.h>
#include "configuration.h"
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

#include <WS2812Serial.h>
#define USE_WS2812SERIAL
#include <FastLED.h>
#include "./fastledGFX/FastLED_GFX.h"
#include "callStack.h"
#include "runner.h"

class DisplayRunner : Runner {
  public: 

    template<int x, int to>
    struct initializeLeds {
        void operator()(DisplayRunner *d) { 
          LEDS.addLeds<WS2812SERIAL,DATA_PINS[x],BRG>(d->leds[x],NUM_LEDS_PER_SECTION);
          initializeLeds<x+1,to>()(d);
        }
    };

    template<int to>
    struct initializeLeds<to,to> {
      void operator()(DisplayRunner *d) {}
    };


    CRGB renderLeds[NUM_LEDS_TOTAL];
    CRGB renderLedsZigzagged[NUM_LEDS_TOTAL];
    CRGB leds[NUM_SECTIONS][NUM_LEDS_PER_SECTION];
    GFXcanvas canvas;

    PeriodicTimer _displayTimer = PeriodicTimer(TCK);
    bool gameLoaded;
    bool gameLoading;
    double loadingProgress = 0;

    DisplayRunner(): canvas(SCREEN_WIDTH, SCREEN_HEIGHT, this) { }
    
    void init(GameState *gameState) {
      Runner::init(gameState);
      initializeLeds<0,NUM_SECTIONS>()(this);
      LEDS.setBrightness(MAX_BRIGHTNESS);
      LEDS.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPS);
      _displayTimer.begin([this]() {this->_displayTrigger(); }, 7 * NUM_LEDS_TOTAL);
    }

    void startLoading(double progress) {
      this->gameLoading = true;
      this->loadingProgress = progress;
      this->force();
    }
    void stopLoading() {
      this->gameLoading = false;
      this->loadingProgress = 0.0;
    }
    void start() {
      this->gameLoaded = true;
      this->gameLoading = false;
    }
    void stop() {
      this->gameLoaded = false;
      this->callStack.clear();
    }
    void force() {
      this->_displayTrigger();
    }

  private: 
    FASTRUN void _displayTrigger() {
      _render();

      if(LAYOUT_IS_ZIG_ZAG) {
        for(unsigned short int column = 0; column < SCREEN_WIDTH; column++) {
          if (column % 2 == 1) {
            CRGB tempLeds[SCREEN_HEIGHT];
            for (int row = 0; row < SCREEN_HEIGHT; row++) {
              tempLeds[(SCREEN_HEIGHT - 1) - row] = renderLeds[column * SCREEN_WIDTH + row];  
            }
            for (int row = 0; row < SCREEN_HEIGHT; row++) {
              renderLedsZigzagged[column * SCREEN_WIDTH + row] = tempLeds[row];  
            }
          } else {
            for (int row = 0; row < SCREEN_HEIGHT; row++) {
              renderLedsZigzagged[column * SCREEN_WIDTH + row] = renderLeds[column * SCREEN_WIDTH + row];  
            }
          }
        }
      } else {
        for(unsigned short int index = 0; index < SCREEN_WIDTH * SCREEN_HEIGHT; index++) {
          renderLedsZigzagged[index] = renderLeds[index];  
        }
      }

      for(unsigned int i = 0; i < NUM_SECTIONS; i++) {
        std::copy(renderLedsZigzagged + (NUM_LEDS_PER_SECTION * i),
          renderLedsZigzagged + (NUM_LEDS_PER_SECTION * i) + NUM_LEDS_PER_SECTION,
          leds[i]);
      }
      FastLED.show();

    };


    FASTRUN void _render();
};

#endif