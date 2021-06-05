#ifndef ARC_CONTROLLER
#define ARC_CONTROLLER
#include <Arduino.h>
#include "i2cController.h"
#include "gameState.h"
#include "displayRunner.h"
#include "mainRunner.h"
#include "../configuration.h"
#include "soundController.h"
#include "gameCardController.h"

class ArcadableController {

  public: 

    DisplayRunner displayRunner;
    MainRunner mainRunner;
    GameState gameState;
    GameCardController gameCardController;

    ArcadableController() { }

    void init() {
        I2cController::init();
        gameState.init();
        displayRunner.init(&gameState);
        mainRunner.init(&gameState);
        gameCardController.init(&gameState, &displayRunner, &mainRunner);

    }


};


#endif