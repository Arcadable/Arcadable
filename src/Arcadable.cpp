#include "Arduino.h"
#include "Arcadable.h"


Arcadable* Arcadable::_instance = NULL;
Arcadable* Arcadable::getInstance() {
	if (!Arcadable::_instance)
    {
        Arcadable::_instance = new Arcadable;
    }

    return Arcadable::_instance;
}

void Arcadable::_mainTrigger() {
    Arcadable::getInstance()->mainStep();
}
void Arcadable::_renderTrigger() {
    Arcadable::getInstance()->renderStep();
}
void Arcadable::_pollTrigger() {
    Arcadable::getInstance()->poll();
}

void Arcadable::setup(
	SystemConfig *systemConfig,
	CRGB *pixels,
	CRGB *pixelsBuffer,
	GFXcanvas* canvas
) {
	this->systemConfig = systemConfig;
	this->pixels = pixels;
	this->canvas = canvas;
	this->pixelsBuffer = pixelsBuffer;
	for (unsigned short int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		this->pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
		this->pixelsBuffer[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}
	Wire.begin();
 	Wire.setClock(systemConfig->wireClock);
    this->_mainTimer = PeriodicTimer(GPT1);
    this->_renderTimer = PeriodicTimer(GPT2);
    this->_pollTimer = PeriodicTimer(TCK);

    _mainTimer.begin(_mainTrigger, systemConfig->targetMainMicros);
    _renderTimer.begin(_renderTrigger, systemConfig->targetRenderMicros);
    _pollTimer.begin(_pollTrigger, 300000);
   // _pollTimer.priority(128);
   // _mainTimer.priority(129);
  //  _renderTimer.priority(130);
}


void Arcadable::poll() {
    Wire.beginTransmission(systemConfig->eepromAddress);
    Wire.write(0);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(static_cast<unsigned int>(systemConfig->eepromAddress), 1);
    delayMicroseconds(100);
    if(_readyToLoad && Wire.available() == 1) {
        _loading = true;
        renderStep();

        if(_gameLoaded) {
            _unloadGameLogic();
        }
        bool readRes = _readAndLoadGameLogic();
        canvas->setRotation(0);
        _readyToLoad = false;
        if (readRes) {
            _gameLoaded = true;
            this->_mainCallStack = CallStack();
            this->_renderCallStack = CallStack();
        } else {
            _gameLoaded = false;
        }
        _loading = false;
        renderStep();
    } else if(Wire.available() == 0) {
        _readyToLoad = true;
    }

}

void Arcadable::mainStep() {
    if(this->_gameLoaded && !this->_loading) {
        this->systemConfig->fetchInputValues();

        this->_mainCallStack.prepareStep();
        this->_mainCallStack.pushfront(this->mainInstructionSet->getExecutables());

        this->_processCallStack(&this->_mainCallStack);
    }

}


void Arcadable::renderStep() {
    if(_gameLoaded && !_loading) {
        this->_renderCallStack.prepareStep();
        this->_renderCallStack.pushfront(this->renderInstructionSet->getExecutables());

        this->_processCallStack(&this->_renderCallStack);
    } else if (!_loading) {
        canvas->fillScreen(CRGB::Black);
        canvas->setRotation(0);
        if((millis() / 500) % 2) {
            canvas->fillTriangle(0, systemConfig->screenHeight, 7, systemConfig->screenHeight, 0, systemConfig->screenHeight - 7, 0x111111);
            canvas->drawLine(0, systemConfig->screenHeight - 1, 6, systemConfig->screenHeight - 7, 0x111111);
            canvas->drawLine(1, systemConfig->screenHeight - 1, 6, systemConfig->screenHeight - 6, 0x111111);
            canvas->drawLine(0, systemConfig->screenHeight - 2, 5, systemConfig->screenHeight - 7, 0x111111);
            canvas->drawLine(0, systemConfig->screenHeight - 3, 5, systemConfig->screenHeight - 8, 0x111111);
            canvas->drawLine(2, systemConfig->screenHeight - 1, 7, systemConfig->screenHeight - 6, 0x111111);

        }
        canvas->setRotation((millis() / 2000) % 4);
        canvas->setTextColor(0xffffff);
        canvas->setTextSize(1);
        canvas->setTextWrap(false);

        canvas->setCursor(systemConfig->screenWidth / 2 - 18, systemConfig->screenHeight / 2 - 10);
        canvas->print("Insert");

        canvas->setCursor(systemConfig->screenWidth / 2 - 12, systemConfig->screenHeight / 2 + 2);
        canvas->print("Card");
    } else {
        canvas->fillScreen(CRGB::Black);
        canvas->drawRect(0, 0, systemConfig->screenWidth, systemConfig->screenHeight, CRGB::White);

    }
     
    if(systemConfig->layoutIsZigZag) {
        for(unsigned short int column = 0; column < systemConfig->screenWidth; column++) {
            if (column % 2 == 1) {
                CRGB tempLeds[systemConfig->screenHeight];
                for (int row = 0; row < systemConfig->screenHeight; row++) {
                    tempLeds[(systemConfig->screenHeight - 1) - row] = pixelsBuffer[column * systemConfig->screenWidth + row];  
                }
                for (int row = 0; row < systemConfig->screenHeight; row++) {
                    pixels[column * systemConfig->screenWidth + row] = tempLeds[row];  
                }
            } else {
                for (int row = 0; row < systemConfig->screenHeight; row++) {
                    pixels[column * systemConfig->screenWidth + row] = pixelsBuffer[column * systemConfig->screenWidth + row];  
                }
            }
        }
    } else {
        for(unsigned short int index = 0; index < systemConfig->screenWidth * systemConfig->screenHeight; index++) {
            pixels[index] = pixelsBuffer[index];  
        }
    }
    FastLED.show();

};

void Arcadable::_processCallStack(CallStack* callStack) {
    if(callStack->size() > 0) {
        Executable* executable = callStack->pop();
        if(executable != NULL) {
            executable->checkWaitMillis();

            if(executable->executeOnMillis > 0) {

                if(executable->executeOnMillis <= millis()) {
                    this->_processExecutable(executable, callStack);
                } else {
                    callStack->delayScheduledSection(executable);
                }
            } else {
                this->_processExecutable(executable, callStack);
            }
        }
        if (callStack->doProcessMore()) {
            this->_processCallStack(callStack);
        }
    }
}

void Arcadable::_processExecutable(Executable* executable, CallStack* callStack) {
    std::vector<Executable> newExecutables = executable->action();
    if(executable->parentAwait != NULL) {
        for ( auto item : newExecutables ) {
            item.withParentAwait(executable->parentAwait);
        }
    }

    if(newExecutables.size() > 0) {
        if(executable->async) {
            if (executable->awaiting.size() > 0) {

                Executable waitFor = Executable([this, executable] () -> const std::vector<Executable>& {
                    std::vector<Executable>* returnExecutables = &executable->awaiting;
                    if(executable->parentAwait != NULL) {
                        for ( auto &item : *returnExecutables ) {
                            item.withParentAwait(executable->parentAwait);
                        }
                    }
                    return *returnExecutables;
                }, true, false, executable->parentAwait, NULL);
                
                for ( auto item : newExecutables ) {
                    item.withParentAwait(&waitFor);
                }
                callStack->pushfront(&newExecutables);


                std::vector<Executable> waitForArray = {waitFor};
                if(executable->parentAwait) {
                    callStack->pushinfrontof(executable->parentAwait, &waitForArray);
                } else {
                    callStack->pushback(&waitForArray);
                }
            } else {
                callStack->pushfront(&newExecutables);
            }
        } else {
            callStack->pushfront(&newExecutables);
        }
    }
}



void Arcadable::_unloadGameLogic() {
	for (unsigned short int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}
	values.clear();
	instructions.clear();
	instructionSets.clear();

}

bool Arcadable::_readAndLoadGameLogic() {
	unsigned int currentParsePosition = 0;

    bool continueReading = true;

    std::map<unsigned short, std::vector<unsigned short>> valueParamsMap;
    std::map<unsigned short, std::vector<unsigned short>> instructionParamsMap;
    std::map<unsigned short, std::vector<unsigned short>> instructionSetParamsMap;

    while (true) {
        if(!continueReading) {
            break;
        }
        unsigned char lengthData[2];
        unsigned short int length;
        bool readRes = _readEEPROM(currentParsePosition, 2, lengthData);
        if(!readRes) {
            return false;
        }
        length = (lengthData[0] << 8) + lengthData[1];
        currentParsePosition += 2;

        unsigned char data[length];
        readRes = _readEEPROM(currentParsePosition, length, data);
        if(!readRes) {
            return false;
        }
        currentParsePosition += length;

        unsigned char type = data[0];
        bool isInstructionType = type >= 128;
        if(isInstructionType) {
            type -= 128;
        }

        bool mainSet = false;
        bool renderSet = false;

        for (unsigned int i = 1 ; i < length ; i += 2) {
            unsigned short id = static_cast<unsigned short>((data[i + 0] << 8) + data[i + 1]);
            if(isInstructionType) {

                switch(type) {
                    case InstructionType::MutateValue: {
                        unsigned short leftValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short rightValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        
                        this->mutateValueInstructions[id] = MutateValueInstruction(id, false);
                        this->instructions[id] = &this->mutateValueInstructions[id];
                        instructionParamsMap[id] = {leftValueID, rightValueID};
                        i += 4;
                        break;
                    }
                    case InstructionType::AwaitedRunCondition: {
                        unsigned short evaluationValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short failSetID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short successSetID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        
                        this->runConditionInstructions[id] = RunConditionInstruction(id, true);
                        this->instructions[id] = &this->runConditionInstructions[id];
                        if(failSetID == 65535) {
                            instructionParamsMap[id] = {evaluationValueID, successSetID};
                        } else {
                            instructionParamsMap[id] = {evaluationValueID, successSetID, failSetID};
                        }
                        i += 6;
                        break;
                    }
                    case InstructionType::RunCondition: {
                        unsigned short evaluationValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short failSetID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short successSetID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        
                        this->runConditionInstructions[id] = RunConditionInstruction(id, false);
                        this->instructions[id] = &this->runConditionInstructions[id];
                        if(failSetID == 65535) {
                            instructionParamsMap[id] = {evaluationValueID, successSetID};
                        } else {
                            instructionParamsMap[id] = {evaluationValueID, successSetID, failSetID};
                        }
                        i += 6;
                        break;
                    }
                    case InstructionType::DrawPixel: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short xValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short yValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        
                        this->drawPixelInstructions[id] = DrawPixelInstruction(id, false);
                        this->instructions[id] = &this->drawPixelInstructions[id];
                        instructionParamsMap[id] = {colorValueID, xValueID, yValueID};
                        i += 6;
                        break;
                    }
                    case InstructionType::DrawLine: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short x1ValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short y1ValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short x2ValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short y2ValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        
                        this->drawLineInstructions[id] = DrawLineInstruction(id, false);
                        this->instructions[id] = &this->drawLineInstructions[id];
                        instructionParamsMap[id] = {colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                        i += 10;
                        break;
                    }
                    case InstructionType::DrawRect: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short x1ValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short y1ValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short x2ValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short y2ValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        
                        this->drawRectInstructions[id] = DrawRectInstruction(id, false);
                        this->instructions[id] = &this->drawRectInstructions[id];
                        instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                        i += 10;
                        break;
                    }
                    case InstructionType::FillRect: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short x1ValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short y1ValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short x2ValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short y2ValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        
                        this->fillRectInstructions[id] = FillRectInstruction(id, false);
                        this->instructions[id] = &this->fillRectInstructions[id];

                        instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                        i += 10;
                        break;
                    }
                    case InstructionType::DrawCircle: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short radiusValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short xValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short yValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        
                        this->drawCircleInstructions[id] = DrawCircleInstruction(id, false);
                        this->instructions[id] = &this->drawCircleInstructions[id];

                        instructionParamsMap[id] = { colorValueID, radiusValueID, xValueID, yValueID};
                        i += 8;
                        break;
                    }
                    case InstructionType::FillCircle: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short radiusValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short xValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short yValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        
                        this->fillCircleInstructions[id] = FillCircleInstruction(id, false);
                        this->instructions[id] = &this->fillCircleInstructions[id];

                        instructionParamsMap[id] = { colorValueID, radiusValueID, xValueID, yValueID};
                        i += 8;
                        break;
                    }
                    case InstructionType::DrawTriangle: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short x1ValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short y1ValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short x2ValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short y2ValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        unsigned short x3ValueID = static_cast<unsigned short>((data[i + 12] << 8) + data[i + 13]);
                        unsigned short y3ValueID = static_cast<unsigned short>((data[i + 14] << 8) + data[i + 15]);
                        
                        this->drawTriangleInstructions[id] = DrawTriangleInstruction(id, false);
                        this->instructions[id] = &this->drawTriangleInstructions[id];

                        instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID, x3ValueID, y3ValueID};
                        i += 14;
                        break;
                    }
                    case InstructionType::FillTriangle: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short x1ValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short y1ValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short x2ValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short y2ValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        unsigned short x3ValueID = static_cast<unsigned short>((data[i + 12] << 8) + data[i + 13]);
                        unsigned short y3ValueID = static_cast<unsigned short>((data[i + 14] << 8) + data[i + 15]);
                        
                        this->fillTriangleInstructions[id] = FillTriangleInstruction(id, false);
                        this->instructions[id] = &this->fillTriangleInstructions[id];
                        instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID, x3ValueID, y3ValueID};
                        i += 14;
                        break;
                    }
                    case InstructionType::DrawImage: {
                        unsigned short xValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short yValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short imageValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);


                        this->drawImageInstructions[id] = DrawImageInstruction(id, false);
                        this->instructions[id] = &this->drawImageInstructions[id];
                        instructionParamsMap[id] = { xValueID, yValueID, imageValueID};
                        i += 6;
                        break;
                    }
                    case InstructionType::DrawText: {
                        unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short xValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short yValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short scaleValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
                        unsigned short textValueID = static_cast<unsigned short>((data[i + 10] << 8) + data[i + 11]);
                        
                        this->drawTextInstructions[id] = DrawTextInstruction(id, false);
                        this->instructions[id] = &this->drawTextInstructions[id];
                        instructionParamsMap[id] = { colorValueID, scaleValueID, textValueID, xValueID, yValueID};
                        i += 10;
                        break;
                    }
                    case InstructionType::Clear: {                
                        this->clearInstructions[id] = ClearInstruction(id, false);
                        this->instructions[id] = &this->clearInstructions[id];
                        break;
                    }
                    case InstructionType::SetRotation: {
                        unsigned short rotationValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        
                        this->setRotationInstructions[id] = SetRotationInstruction(id, false);
                        this->instructions[id] = &this->setRotationInstructions[id];

                        instructionParamsMap[id] = { rotationValueID};
                        i += 2;
                        break;
                    }
                    case InstructionType::AwaitedRunSet: {
                        unsigned short setID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        
                        this->runSetInstructions[id] = RunSetInstruction(id, true);
                        this->instructions[id] = &this->runSetInstructions[id];
                        instructionParamsMap[id] = { setID};
                        i += 2;
                        break;
                    }
                    case InstructionType::RunSet: {
                        unsigned short setID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        
                        this->runSetInstructions[id] = RunSetInstruction(id, false);
                        this->instructions[id] = &this->runSetInstructions[id];
                        instructionParamsMap[id] = { setID};
                        i += 2;
                        break;
                    }
                    case InstructionType::DebugLog: {
                        unsigned short logValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        
                        this->debugLogInstructions[id] = DebugLogInstruction(id, false);
                        this->instructions[id] = &this->debugLogInstructions[id];

                        instructionParamsMap[id] = { logValueID};
                        i += 2;
                        break;
                    }
                    case InstructionType::Wait: {
                        unsigned short amountValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        
                        this->waitInstructions[id] = WaitInstruction(id, true);
                        this->instructions[id] = &this->waitInstructions[id];

                        instructionParamsMap[id] = { amountValueID };
                        i += 2;
                        break;
                    }
                    case InstructionType::Tone: {
                        unsigned short volumeValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short frequencyValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short durationValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);

                        this->toneInstructions[id] = ToneInstruction(id, false);
                        this->instructions[id] = &this->toneInstructions[id];

                        instructionParamsMap[id] = { volumeValueID, frequencyValueID, durationValueID };
                        i += 6;
                        break;
                    }
                    case InstructionType::AwaitedTone: {
                        unsigned short volumeValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short frequencyValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short durationValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);

                        this->toneInstructions[id] = ToneInstruction(id, true);
                        this->instructions[id] = &this->toneInstructions[id];

                        instructionParamsMap[id] = { volumeValueID, frequencyValueID, durationValueID };
                        i += 6;
                        break;
                    }
                    case InstructionType::InstructionSetType: {
                        unsigned short size = static_cast<unsigned short>(((data[i + 2] << 8) & 0b1111111) + data[i + 3]);
                        bool async = static_cast<bool>((data[i + 2] >> 7) & 0b1);

                        instructionSetParamsMap[id] = std::vector<unsigned short>(size);
                        for(unsigned short vI = 0; vI < size * 2; vI += 2) {
                            unsigned short instructionID = static_cast<unsigned short>((data[i + 4 + vI] << 8) + data[i + 5 + vI]);
                            instructionSetParamsMap[id][vI / 2] = instructionID;
                        }
                        this->instructionSets[id] = InstructionSet(id, size, async);
                        if (!mainSet) {
                            mainInstructionSet = &this->instructionSets[id];
                            mainSet = true;
                        } else if (!renderSet) {
                            renderInstructionSet = &this->instructionSets[id];
                            renderSet = true;
                        }

                        i += (2 + (size * 2));
                        continueReading = false;
                        break;
                    }
                    default: break;
                }
            } else {

                switch(type) {
                    case ValueType::number: {
                        #pragma GCC diagnostic push
                        #pragma GCC diagnostic ignored "-Wstrict-aliasing"
                        float f;
                        *((int*) &f) = (data[i + 2] << 24) + (data[i + 3] << 16) + (data[i + 4] << 8) + (data[i + 5]);
                        #pragma GCC diagnostic pop
                        this->numberValues[id] = NumberValue(id, f, 4);
                        this->values[id] = &this->numberValues[id];
                        i += 4;
                        break;
                    }
                    case ValueType::pixelIndex: {
                        unsigned short xID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short yID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        
                        this->pixelValues[id] = PixelValue(id);
                        this->values[id] = &this->pixelValues[id];
                        valueParamsMap[id] = {xID, yID};
                        i += 4;
                        break;
                    }
                    case ValueType::image: {
                        unsigned short dataID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short widthID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        unsigned short heightID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
                        unsigned short keyColorID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);

                        this->imageValues[id] = ImageValue(id);
                        this->values[id] = &this->imageValues[id];
                        valueParamsMap[id] = {dataID, widthID, heightID, keyColorID};
                        i += 8;
                        break;
                    }
                    case ValueType::data: {
                        unsigned short size = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        valueParamsMap[id] = std::vector<unsigned short>(size);
                        for(unsigned short vI = 0; vI < size; vI += 1) {
                            unsigned short valueID = static_cast<unsigned short>(data[i + 4 + vI]);
                            valueParamsMap[id][vI] = valueID;
                        }


                        this->dataValues[id] = DataValue(id, size);
                        this->values[id] = &this->dataValues[id];
                        i += (2 + size);
                        break;
                    }
                    case ValueType::digitalInputPointer: {
                        unsigned short index = static_cast<unsigned short>(data[i + 2]);
                        
                        this->digitalInputValues[id] = DigitalInputValue(id, index);
                        this->values[id] = &this->digitalInputValues[id];
                        i += 1;
                        break;
                    }
                    case ValueType::analogInputPointer: {
                        unsigned short index = static_cast<unsigned short>(data[i + 2]);
                        
                        this->analogInputValues[id] = AnalogInputValue(id, index);
                        this->values[id] = &this->analogInputValues[id];
                        i += 1;
                        break;
                    }
                    case ValueType::systemPointer: {
                        SystemConfigType configType = static_cast<SystemConfigType>(data[i + 2]);
                        
                        this->systemConfigValues[id] = SystemConfigValue(id, configType);
                        this->values[id] = &this->systemConfigValues[id];
                        i += 1;
                        break;
                    }

                    case ValueType::listValue: {
                        unsigned short listValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        unsigned short listIndexID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
                        
                        this->listValues[id] = ListValue(id);
                        this->values[id] = &this->listValues[id];
                        valueParamsMap[id] = {listValueID, listIndexID};
                        i += 4;
                        break;
                    }
                    case ValueType::text: {
                        unsigned char size = static_cast<unsigned char>(data[i + 2]);
                        valueParamsMap[id] = std::vector<unsigned short>(size);
                        for(unsigned short vI = 0; vI < size * 2; vI += 2) {
                            unsigned short valueID = static_cast<unsigned short>((data[i + 3 + vI] << 8) + data[i + 4 + vI]);
                            valueParamsMap[id][vI / 2] = valueID;
                        }
                        
                        this->textValues[id] = TextValue(id, size);
                        this->values[id] = &this->textValues[id];

                        i += (1 + (size * 2));
                        break;
                    }
                    case ValueType::listDeclaration: {
                        unsigned short size = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
                        valueParamsMap[id] = std::vector<unsigned short>(size);
                        for(unsigned short vI = 0; vI < size * 2; vI += 2) {
                            unsigned short valueID = static_cast<unsigned short>((data[i + 4 + vI] << 8) + data[i + 5 + vI]);
                            valueParamsMap[id][vI / 2] = valueID;
                        }
                        this->listDeclarations[id] = ListDeclaration(id, size);
                        this->values[id] = &this->listDeclarations[id];
                        i += (2 + (size * 2));
                        break;
                    }
                    case ValueType::evaluation: {
                        EvaluationOperator evaluationOperator = static_cast<EvaluationOperator>(data[i + 2] >> 1);
                        bool isStatic = static_cast<bool>(data[i + 2] & 0b1);
                        unsigned short leftID = static_cast<unsigned short>((data[i + 3] << 8) + data[i + 4]);
                        unsigned short rightID = static_cast<unsigned short>((data[i + 5] << 8) + data[i + 6]);

                        this->evaluationValues[id] = EvaluationValue(
                            id,
                            evaluationOperator,
                            isStatic
                        );
                        this->values[id] = &this->evaluationValues[id];
                        valueParamsMap[id] = {leftID, rightID};
                        i += 5;
                        break;
                    }
                    default: break;
                }
            }

        }
        
    }
    for (auto x : valueParamsMap) {
        if(this->values[x.first]->type != ValueType::listValue ) {
            this->values[x.first]->init(x.second);
        }
    }

    for (auto x : valueParamsMap) {
        if(this->values[x.first]->type == ValueType::listValue) {
            this->values[x.first]->init(x.second);
        }
    }

    for (auto x : instructionParamsMap) {
        this->instructions[x.first]->init(x.second);
    }

    for (auto x : instructionSetParamsMap) {
        this->instructionSets[x.first].init(x.second);
    }

	this->systemConfig->startMillis = millis();
    return true;
}

bool Arcadable::_readEEPROM(unsigned int startAddress, unsigned int dataLength, unsigned char *dataReceiver) {
    unsigned int currentAddress = startAddress;
    unsigned int currentReadIndex = 0;
    float good = 0;
    float corrected = 0;
    while (currentReadIndex < dataLength) {

        unsigned int readLength = systemConfig->eepromReadWriteBufferSize;
        if (dataLength - currentReadIndex < systemConfig->eepromReadWriteBufferSize) {
            readLength = dataLength - currentReadIndex; 
        }
        unsigned char readBuffer1[readLength];
        unsigned char readBuffer2[readLength];
        unsigned char readBuffer3[readLength];

        _readEEPROMBlock(currentAddress, readLength, readBuffer1);
        _readEEPROMBlock(currentAddress, readLength, readBuffer2);
        _readEEPROMBlock(currentAddress, readLength, readBuffer3);
        for(unsigned char i = 0; i < readLength; i++) {
            unsigned char first = readBuffer1[i];
            unsigned char second = readBuffer2[i];
            unsigned char third = readBuffer3[i];
            if(!(first == second && second == third)) {
                corrected++;
            } else {
                good++;
            }
            unsigned char corrected =
                ((((first & 0b10000000) >> 7) + ((second & 0b10000000) >> 7) + ((third & 0b10000000) >> 7) >= 2 ? 1 : 0) << 7) +
                ((((first & 0b01000000) >> 6) + ((second & 0b01000000) >> 6) + ((third & 0b01000000) >> 6) >= 2 ? 1 : 0) << 6) +
                ((((first & 0b00100000) >> 5) + ((second & 0b00100000) >> 5) + ((third & 0b00100000) >> 5) >= 2 ? 1 : 0) << 5) +
                ((((first & 0b00010000) >> 4) + ((second & 0b00010000) >> 4) + ((third & 0b00010000) >> 4) >= 2 ? 1 : 0) << 4) +
                ((((first & 0b00001000) >> 3) + ((second & 0b00001000) >> 3) + ((third & 0b00001000) >> 3) >= 2 ? 1 : 0) << 3) +
                ((((first & 0b00000100) >> 2) + ((second & 0b00000100) >> 2) + ((third & 0b00000100) >> 2) >= 2 ? 1 : 0) << 2) +
                ((((first & 0b00000010) >> 1) + ((second & 0b00000010) >> 1) + ((third & 0b00000010) >> 1) >= 2 ? 1 : 0) << 1) +
                ((((first & 0b00000001) >> 0) + ((second & 0b00000001) >> 0) + ((third & 0b00000001) >> 0) >= 2 ? 1 : 0) << 0);
            dataReceiver[currentReadIndex + i] = corrected;
        }

        currentAddress += systemConfig->eepromReadWriteBufferSize; 
        currentReadIndex += systemConfig->eepromReadWriteBufferSize;
    };
    delayMicroseconds(10);
    if(corrected / (good + corrected) > 0) {
        return false;
    }
    return true;
}

void Arcadable::_readEEPROMBlock(unsigned int startAddress, unsigned int readLength, unsigned char *dataReceiver) {
    bool addressWritten = false;
    while (!addressWritten) {
        Wire.beginTransmission(systemConfig->eepromAddress);
        Wire.write(startAddress >> 8);
        Wire.write(startAddress & 0xFF);
        addressWritten = Wire.endTransmission() == 0;
    }

    Wire.requestFrom(systemConfig->eepromAddress, readLength);
    int waitingTime = 0;
    while(!Wire.available() && waitingTime <= 10000) {
        delayMicroseconds(10);
        waitingTime++;
    }

    int i = 0;
    while(Wire.available()) {
        dataReceiver[i] = Wire.read();
        i++;
    }
}