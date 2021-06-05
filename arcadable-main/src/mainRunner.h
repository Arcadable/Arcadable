#ifndef _ARC_MAIN_RUNNER
#define _ARC_MAIN_RUNNER

#include <Arduino.h>
#include "../configuration.h"
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;

#include "digitalReader.h"
#include "analogReader.h"
#include "callStack.h"
#include "runner.h"
#include "soundController.h"

class MainRunner : Runner {
  public: 
 
    DigitalReader digitalReader;
    AnalogReader analogReader;
    PeriodicTimer _mainTimer = PeriodicTimer(TCK);
    bool gameLoaded;
    bool first;
    elapsedMicros mainDelta;

    MainRunner() {

    }
    void init(GameState *gameState) {
      Runner::init(gameState);
      this->digitalReader.init();
      this->analogReader.init();
      this->_mainTimer.begin([this]() {this->_mainTrigger(); }, 1);
    }

    void start() {
      this->gameLoaded = true;
      this->first = true;
      this->mainDelta = 0;
    }
    void stop() {
      this->gameLoaded = false;
      this->callStack.clear();
    }

  private: 
    FASTRUN void _mainTrigger();

};

#endif