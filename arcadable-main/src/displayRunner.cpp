#include "displayRunner.h"
#include "gameState.h"

FASTRUN void DisplayRunner::_render() {
    if(this->gameLoaded && !this->gameLoading) {
        this->callStack.prepareStep();
        this->callStack.pushfront(this->gameState->renderInstructionSet->getExecutables());
        this->_processCallStack(&this->callStack);
    } else if (!this->gameLoading) {
        this->canvas.fillScreen(CRGB::Black);
        this->canvas.setRotation(0);
        if((millis() / 500) % 2) {
            this->canvas.fillTriangle(0, SCREEN_HEIGHT, 7, SCREEN_HEIGHT, 0, SCREEN_HEIGHT - 7, 0x111111);
            this->canvas.drawLine(0, SCREEN_HEIGHT - 1, 6, SCREEN_HEIGHT - 7, 0x111111);
            this->canvas.drawLine(1, SCREEN_HEIGHT - 1, 6, SCREEN_HEIGHT - 6, 0x111111);
            this->canvas.drawLine(0, SCREEN_HEIGHT - 2, 5, SCREEN_HEIGHT - 7, 0x111111);
            this->canvas.drawLine(0, SCREEN_HEIGHT - 3, 5, SCREEN_HEIGHT - 8, 0x111111);
            this->canvas.drawLine(2, SCREEN_HEIGHT - 1, 7, SCREEN_HEIGHT - 6, 0x111111);

        }
        this->canvas.setRotation((millis() / 2000) % 4);
        this->canvas.setTextColor(0xffffff);
        this->canvas.setTextSize(1);
        this->canvas.setTextWrap(false);

        this->canvas.setCursor(SCREEN_WIDTH / 2 - 18, SCREEN_HEIGHT / 2 - 10);
        this->canvas.print("Insert");

        this->canvas.setCursor(SCREEN_WIDTH / 2 - 12, SCREEN_HEIGHT / 2 + 2);
        this->canvas.print("Card");
    } else {
        if(this->timeSinceProgress > 2000) {
            this->gameLoading = false;
        } else {
            this->canvas.fillScreen(CRGB::Black);
            unsigned char width = static_cast<unsigned char>(static_cast<double>(SCREEN_WIDTH) * this->loadingProgress);
            unsigned char height = static_cast<unsigned char>(static_cast<double>(SCREEN_HEIGHT) * this->loadingProgress);
            this->canvas.drawRect((SCREEN_WIDTH / 2) - (width / 2), (SCREEN_HEIGHT / 2) - (height / 2), width, height, CRGB::White);
        }
    }
}