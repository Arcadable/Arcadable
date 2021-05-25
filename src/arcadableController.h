#ifndef ARC_CONTROLLER
#define ARC_CONTROLLER
#include <Arduino.h>
#include "i2cController.h"
#include "gameState.h"
#include "displayRunner.h"
#include "mainRunner.h"
#include "configuration.h"
#include "soundController.h"
#include "gameCardController.h"

class ArcadableController {

  public: 

    I2cController i2cController;
    DisplayRunner displayRunner;
    MainRunner mainRunner;
    GameState gameState;
    SoundController soundController;
    GameCardController gameCardController;

    ArcadableController() { }

    void init() {
        i2cController.init();
        soundController.init(&i2cController);
        gameState.init();
        displayRunner.init(&gameState);
        mainRunner.init(&gameState, &soundController);
        gameCardController.init(&gameState, &i2cController, &displayRunner, &mainRunner, &soundController);

    }


};


#endif