#include "Arduino.h"
#include "Arcadable.h"


Arcadable *Arcadable::_instance = NULL;
Arcadable *Arcadable::getInstance() {
	if (!_instance)
    {
        _instance = new Arcadable;
    }

    return _instance;
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

    _mainTimer.begin(_mainTrigger, systemConfig->targetMainMillis * 1000);
    _renderTimer.begin(_renderTrigger, systemConfig->targetRenderMillis * 1000);
    _pollTimer.begin(_pollTrigger, 1000000);
    _pollTimer.priority(0);
    _mainTimer.priority(1);
    _renderTimer.priority(2);
}


void Arcadable::poll() {
    Wire.beginTransmission(systemConfig->eepromAddress);
    Wire.write(0);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(static_cast<unsigned int>(systemConfig->eepromAddress), 1);
    delay(1);
    if(_readyToLoad && Wire.available() == 1) {
        if(_gameLoaded) {
            _unloadGameLogic();
        }
        _readAndLoadGameLogic();
        _readyToLoad = false;
        _gameLoaded = true;
    } else if(Wire.available() == 0) {
        _readyToLoad = true;
    }

}

void Arcadable::mainStep() {

    if(_gameLoaded) {

        systemConfig->fetchInputValues();
        mainInstructionSet->execute();
    }

}

void Arcadable::renderStep() {

    if(_gameLoaded) {
        Arcadable::getInstance()->renderInstructionSet->execute();
    } else {
        canvas->setTextColor(0xffffff);
        canvas->setTextSize(1);
        canvas->setTextWrap(false);
        canvas->fillScreen(CRGB::Black);

        canvas->setCursor(systemConfig->screenWidth / 2 - 18, systemConfig->screenHeight / 2 - 10);
        canvas->print("Insert");

        canvas->setCursor(systemConfig->screenWidth / 2 - 12, systemConfig->screenHeight / 2 + 2);
        canvas->print("Card");
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


void Arcadable::_unloadGameLogic() {
	for (unsigned short int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}
	values.clear();
	instructions.clear();
	instructionSets.clear();

}

void Arcadable::_readAndLoadGameLogic() {
	unsigned int currentParsePosition = 0;
	unsigned char valuesLengthData[2];
	_readEEPROM(currentParsePosition, 2, valuesLengthData);
	unsigned short int valuesLength = (valuesLengthData[0] << 8) + valuesLengthData[1];
	currentParsePosition += 2;

	unsigned char valuesData[valuesLength];
	_readEEPROM(currentParsePosition, valuesLength, valuesData);
	currentParsePosition += valuesLength;

    std::map<unsigned short, std::vector<unsigned short>> valueParamsMap;

	for (unsigned int i = 0 ; i < sizeof(valuesData)/sizeof(valuesData[0]) ; i += 3) {
		unsigned short id = static_cast<unsigned short>((valuesData[i + 0] << 8) + valuesData[i + 1]);
		ValueType type = static_cast<ValueType>(valuesData[i + 2]);
        
        switch(type) {
            case ValueType::number: {
                unsigned short size = static_cast<unsigned short>(valuesData[i + 3]);
				float f;
				*((int*) &f) = (valuesData[i + 4] << 24) + (valuesData[i + 5] << 16) + (valuesData[i + 6] << 8) + (valuesData[i + 7]);
                
                this->numberValues[id] = NumberValue(id, f, size);
                this->values[id] = &this->numberValues[id];
                i += 5;
                break;
            }
            case ValueType::pixelIndex: {
                unsigned short xID = static_cast<unsigned short>((valuesData[i + 3] << 8) + valuesData[i + 4]);
                unsigned short yID = static_cast<unsigned short>((valuesData[i + 5] << 8) + valuesData[i + 6]);
                
                this->pixelValues[id] = PixelValue(id);
                this->values[id] = &this->pixelValues[id];
                valueParamsMap[id] = {xID, yID};
                i += 4;
                break;
            }
            case ValueType::digitalInputPointer: {
                unsigned short index = static_cast<unsigned short>(valuesData[i + 3]);
                
                this->digitalInputValues[id] = DigitalInputValue(id, index);
                this->values[id] = &this->digitalInputValues[id];
                i += 1;
                break;
            }
            case ValueType::analogInputPointer: {
                unsigned short index = static_cast<unsigned short>(valuesData[i + 3]);
                
                this->analogInputValues[id] = AnalogInputValue(id, index);
                this->values[id] = &this->analogInputValues[id];
                i += 1;
                break;
            }
            case ValueType::systemPointer: {
                SystemConfigType configType = static_cast<SystemConfigType>(valuesData[i + 3]);
                
                this->systemConfigValues[id] = SystemConfigValue(id, configType);
                this->values[id] = &this->systemConfigValues[id];
                i += 1;
                break;
            }

            case ValueType::listValue: {
                unsigned short listValueID = static_cast<unsigned short>((valuesData[i + 3] << 8) + valuesData[i + 4]);
                unsigned short listIndexID = static_cast<unsigned short>((valuesData[i + 5] << 8) + valuesData[i + 6]);
                
                this->listValues[id] = ListValue(id);
                this->values[id] = &this->listValues[id];
                valueParamsMap[id] = {listValueID, listIndexID};
                i += 4;
                break;
            }
            case ValueType::text: {
                unsigned char size = static_cast<unsigned char>(valuesData[i + 3]);
                valueParamsMap[id] = std::vector<unsigned short>(size);
                for(unsigned short vI = 0; vI < size * 2; vI += 2) {
                    unsigned short valueID = static_cast<unsigned short>((valuesData[i + 4 + vI] << 8) + valuesData[i + 5 + vI]);
                    valueParamsMap[id][vI / 2] = valueID;
                }
                
                this->textValues[id] = TextValue(id, size);
                this->values[id] = &this->textValues[id];
                i += (1 + (size * 2));
                break;
            }
            case ValueType::listDeclaration: {
                unsigned short size = static_cast<unsigned short>((valuesData[i + 3] << 8) + valuesData[i + 4]);
                valueParamsMap[id] = std::vector<unsigned short>(size);
                for(unsigned short vI = 0; vI < size * 2; vI += 2) {
                    unsigned short valueID = static_cast<unsigned short>((valuesData[i + 5 + vI] << 8) + valuesData[i + 6 + vI]);
                    valueParamsMap[id][vI / 2] = valueID;
                }
                this->listDeclarations[id] = ListDeclaration(id, size);
                this->values[id] = &this->listDeclarations[id];
                i += (2 + (size * 2));
                break;
            }
            case ValueType::evaluation: {
                EvaluationOperator evaluationOperator = static_cast<EvaluationOperator>(valuesData[i + 3] >> 1);
		        bool isStatic = static_cast<bool>(valuesData[i + 3] & 0b1);
                unsigned short leftID = static_cast<unsigned short>((valuesData[i + 4] << 8) + valuesData[i + 5]);
                unsigned short rightID = static_cast<unsigned short>((valuesData[i + 6] << 8) + valuesData[i + 7]);

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

	unsigned char instructionsLengthData[2];
	_readEEPROM(currentParsePosition, 2, instructionsLengthData);
	unsigned short int instructionsLength = (instructionsLengthData[0] << 8) + instructionsLengthData[1];
	currentParsePosition += 2;

	unsigned char instructionsData[instructionsLength];
	_readEEPROM(currentParsePosition, instructionsLength, instructionsData);
	currentParsePosition += instructionsLength;
    std::map<unsigned short, std::vector<unsigned short>> instructionParamsMap;
	for (unsigned int i = 0 ; i < sizeof(instructionsData)/sizeof(instructionsData[0]) ; i += 3) {
		unsigned short id = static_cast<unsigned short>((instructionsData[i + 0] << 8) + instructionsData[i + 1]);
		InstructionType type = static_cast<InstructionType>(instructionsData[i + 2]);

        switch(type) {
            case InstructionType::MutateValue: {
                unsigned short leftValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short rightValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                
                this->mutateValueInstructions[id] = MutateValueInstruction(id);
                this->instructions[id] = &this->mutateValueInstructions[id];
                instructionParamsMap[id] = {leftValueID, rightValueID};
                i += 4;
                break;
            }
            case InstructionType::RunCondition: {
                unsigned short evaluationValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short failSetID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short successSetID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                
                this->runConditionInstructions[id] = RunConditionInstruction(id);
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
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short xValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short yValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                
                this->drawPixelInstructions[id] = DrawPixelInstruction(id);
                this->instructions[id] = &this->drawPixelInstructions[id];
                instructionParamsMap[id] = {colorValueID, xValueID, yValueID};
                i += 6;
                break;
            }
            case InstructionType::DrawLine: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short x1ValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short y1ValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short x2ValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short y2ValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                
                this->drawLineInstructions[id] = DrawLineInstruction(id);
                this->instructions[id] = &this->drawLineInstructions[id];
                instructionParamsMap[id] = {colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                i += 10;
                break;
            }
            case InstructionType::DrawRect: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short x1ValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short y1ValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short x2ValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short y2ValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                
                this->drawRectInstructions[id] = DrawRectInstruction(id);
                this->instructions[id] = &this->drawRectInstructions[id];
                instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                i += 10;
                break;
            }
            case InstructionType::FillRect: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short x1ValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short y1ValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short x2ValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short y2ValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                
                this->fillRectInstructions[id] = FillRectInstruction(id);
                this->instructions[id] = &this->fillRectInstructions[id];

                instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
                i += 10;
                break;
            }
            case InstructionType::DrawCircle: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short radiusValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short xValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short yValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                
                this->drawCircleInstructions[id] = DrawCircleInstruction(id);
                this->instructions[id] = &this->drawCircleInstructions[id];

                instructionParamsMap[id] = { colorValueID, radiusValueID, xValueID, yValueID};
                i += 8;
                break;
            }
            case InstructionType::FillCircle: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short radiusValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short xValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short yValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                
                this->fillCircleInstructions[id] = FillCircleInstruction(id);
                this->instructions[id] = &this->fillCircleInstructions[id];

                instructionParamsMap[id] = { colorValueID, radiusValueID, xValueID, yValueID};
                i += 8;
                break;
            }
            case InstructionType::DrawTriangle: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short x1ValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short y1ValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short x2ValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short y2ValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                unsigned short x3ValueID = static_cast<unsigned short>((instructionsData[i + 13] << 8) + instructionsData[i + 14]);
                unsigned short y3ValueID = static_cast<unsigned short>((instructionsData[i + 15] << 8) + instructionsData[i + 16]);
                
                this->drawTriangleInstructions[id] = DrawTriangleInstruction(id);
                this->instructions[id] = &this->drawTriangleInstructions[id];

                instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID, x3ValueID, y3ValueID};
                i += 14;
                break;
            }
            case InstructionType::FillTriangle: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short x1ValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short y1ValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short x2ValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short y2ValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                unsigned short x3ValueID = static_cast<unsigned short>((instructionsData[i + 13] << 8) + instructionsData[i + 14]);
                unsigned short y3ValueID = static_cast<unsigned short>((instructionsData[i + 15] << 8) + instructionsData[i + 16]);
                
                this->fillTriangleInstructions[id] = FillTriangleInstruction(id);
                this->instructions[id] = &this->fillTriangleInstructions[id];
                instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID, x3ValueID, y3ValueID};
                i += 14;
                break;
            }
            case InstructionType::DrawText: {
                unsigned short colorValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                unsigned short xValueID = static_cast<unsigned short>((instructionsData[i + 5] << 8) + instructionsData[i + 6]);
                unsigned short yValueID = static_cast<unsigned short>((instructionsData[i + 7] << 8) + instructionsData[i + 8]);
                unsigned short scaleValueID = static_cast<unsigned short>((instructionsData[i + 9] << 8) + instructionsData[i + 10]);
                unsigned short textValueID = static_cast<unsigned short>((instructionsData[i + 11] << 8) + instructionsData[i + 12]);
                
                this->drawTextInstructions[id] = DrawTextInstruction(id);
                this->instructions[id] = &this->drawTextInstructions[id];
                instructionParamsMap[id] = { colorValueID, scaleValueID, textValueID, xValueID, yValueID};
                i += 10;
                break;
            }
            case InstructionType::Clear: {                
                this->clearInstructions[id] = ClearInstruction(id);
                this->instructions[id] = &this->clearInstructions[id];
                break;
            }
            case InstructionType::SetRotation: {
                unsigned short rotationValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                
                this->setRotationInstructions[id] = SetRotationInstruction(id);
                this->instructions[id] = &this->setRotationInstructions[id];

                instructionParamsMap[id] = { rotationValueID};
                i += 2;
                break;
            }
            case InstructionType::RunSet: {
                unsigned short setID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                
                this->runSetInstructions[id] = RunSetInstruction(id);
                this->instructions[id] = &this->runSetInstructions[id];
                instructionParamsMap[id] = { setID};
                i += 2;
                break;
            }
            case InstructionType::DebugLog: {
                unsigned short logValueID = static_cast<unsigned short>((instructionsData[i + 3] << 8) + instructionsData[i + 4]);
                
                this->debugLogInstructions[id] = DebugLogInstruction(id);
                this->instructions[id] = &this->debugLogInstructions[id];

                instructionParamsMap[id] = { logValueID};
                i += 2;
                break;
            }
            default: break;
        }
    }

    for (auto x : instructionParamsMap) {
        this->instructions[x.first]->init(x.second);
    }

	unsigned char instructionSetsLengthData[2];
	_readEEPROM(currentParsePosition, 2, instructionSetsLengthData);
	unsigned short int instructionSetsLength = (instructionSetsLengthData[0] << 8) + instructionSetsLengthData[1];
	currentParsePosition += 2;

	unsigned char instructionSetsData[instructionSetsLength];
	_readEEPROM(currentParsePosition, instructionSetsLength, instructionSetsData);
	currentParsePosition += instructionSetsLength;
    std::map<unsigned short, std::vector<unsigned short>> instructionSetParamsMap;
    
    bool mainSet = false;
    bool renderSet = false;
	for (unsigned int i = 0 ; i < sizeof(instructionSetsData)/sizeof(instructionSetsData[0]) ; i += 4) {
		unsigned short id = static_cast<unsigned short>((instructionSetsData[i + 0] << 8) + instructionSetsData[i + 1]);
		unsigned short size = static_cast<unsigned short>((instructionSetsData[i + 2] << 8) + instructionSetsData[i + 3]);

        instructionSetParamsMap[id] = std::vector<unsigned short>(size);
        for(unsigned short vI = 0; vI < size * 2; vI += 2) {
            unsigned short instructionID = static_cast<unsigned short>((instructionSetsData[i + 4 + vI] << 8) + instructionSetsData[i + 5 + vI]);
            instructionSetParamsMap[id][vI / 2] = instructionID;
        }
        this->instructionSets[id] = InstructionSet(id, size);
        if (!mainSet) {
            mainInstructionSet = &this->instructionSets[id];
            mainSet = true;
        } else if (!renderSet) {
            renderInstructionSet = &this->instructionSets[id];
            renderSet = true;
        }

        i += (size * 2);
    }

    for (auto x : instructionSetParamsMap) {
        this->instructionSets[x.first].init(x.second);
    }
	this->systemConfig->startMillis = millis();

}

void Arcadable::_readEEPROM(unsigned int startAddress, unsigned int dataLength, unsigned char *dataReceiver) {
  unsigned int currentAddress = startAddress;
  unsigned int currentReadIndex = 0;
  while (currentReadIndex < dataLength) {
    bool addressWritten = false;
		while (!addressWritten) {
			Wire.beginTransmission(systemConfig->eepromAddress);
			Wire.write(currentAddress >> 8);
			Wire.write(currentAddress & 0xFF);
			addressWritten = Wire.endTransmission() == 0;
		}

    unsigned int readLength = systemConfig->eepromReadWriteBufferSize;
    if (dataLength - currentReadIndex < systemConfig->eepromReadWriteBufferSize) {
      readLength = dataLength - currentReadIndex; 
    }

    Wire.requestFrom(systemConfig->eepromAddress, readLength);
    int waitingTime = 0;
    while(!Wire.available() && waitingTime <= 100) {
      delay(1);
      waitingTime++;
    }

    int i = 0;
    while(Wire.available()) {
      dataReceiver[currentReadIndex + i] = Wire.read();
      i++;
    }
    currentAddress += systemConfig->eepromReadWriteBufferSize; 
    currentReadIndex += systemConfig->eepromReadWriteBufferSize;
		delay(1);
  };
}