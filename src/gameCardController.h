#ifndef _ARC_CARD_CONTROLLER
#define _ARC_CARD_CONTROLLER

#include <Arduino.h>
#include "configuration.h"
#include "TeensyTimerTool.h"
using namespace TeensyTimerTool;
#include "gameState.h"
#include "i2cController.h"
#include "mainRunner.h"

class GameCardController {
  public: 
 
    PeriodicTimer _pollTimer = PeriodicTimer(TCK);
    GameState *gameState;
    I2cController *i2cController;
	DisplayRunner *displayRunner;
	MainRunner *mainRunner;
	SoundController *soundController;
	bool gameLoaded;
	bool readyToLoad;

    GameCardController() {

    }
    void init(GameState *gameState, I2cController *i2cController, DisplayRunner *displayRunner, MainRunner *mainRunner, SoundController *soundController) {
      	this->gameState = gameState;
      	this->i2cController = i2cController;
		this->displayRunner = displayRunner;
		this->mainRunner = mainRunner;
      	_pollTimer.begin([this]() {this->_pollTrigger(); }, GAMECARD_POLLING_INTERVAL * 1000);
		this->gameLoaded = false;
		this->readyToLoad = true;
    }



  private: 
    void _pollTrigger() {
		
      	if(this->readyToLoad) {
			if(this->i2cController->isAvailable(GAMECARD_EEPROM_ADDRESS)) {
				this->readyToLoad = false;
				this->displayRunner->startLoading(0.01);
				if(this->gameLoaded) {
					this->_unloadGameLogic();
				}
				elapsedMillis wait = 0;
				while(wait < 500) {}
				if(this->i2cController->isAvailable(GAMECARD_EEPROM_ADDRESS)) {
					bool readRes = this->_readAndLoadGameLogic();
					this->displayRunner->canvas.setRotation(0);
					if (readRes) {
						this->gameLoaded = true;
						this->mainRunner->start();
						this->displayRunner->start();
					} else {
						this->gameLoaded = false;
					}
					this->displayRunner->stopLoading();
				}
			}
      	} else if(!this->i2cController->isAvailable(GAMECARD_EEPROM_ADDRESS)) {
			this->readyToLoad = true;
		}
    }

	void _unloadGameLogic() {
		this->gameState->values.clear();
		this->gameState->instructions.clear();
		this->gameState->instructionSets.clear();
	}

    bool _readAndLoadGameLogic() {
		unsigned int currentParsePosition = 0;
		bool continueReading = true;
		std::map<unsigned short, std::vector<unsigned short>> valueParamsMap;
		std::map<unsigned short, std::vector<unsigned short>> instructionParamsMap;
		std::map<unsigned short, std::vector<unsigned short>> instructionSetParamsMap;

		double p = 0.0;
		while (true) {
			if(!continueReading) {
				break;
			}
			p = p + 1.0;
			this->displayRunner->startLoading(p / 34.0);
			unsigned char lengthData[2];
			unsigned short int length;
			bool readRes = this->i2cController->read(GAMECARD_EEPROM_ADDRESS, currentParsePosition, 2, lengthData);
			if(!readRes) {
				return false;
			}
			length = (lengthData[0] << 8) + lengthData[1];
			currentParsePosition += 2;

			unsigned char data[length];
			readRes = this->i2cController->read(GAMECARD_EEPROM_ADDRESS, currentParsePosition, length, data);
			if(!readRes) {
				return false;
			}
			currentParsePosition += length;

			unsigned char type = data[0];
			bool isInstructionType = type >= 128;
			if(isInstructionType) {
				type -= 128;
			}
			bool setupSet = false;
			bool mainSet = false;
			bool renderSet = false;

			for (unsigned int i = 1 ; i < length ; i += 2) {
				unsigned short id = static_cast<unsigned short>((data[i + 0] << 8) + data[i + 1]);
				if(isInstructionType) {                
	

					switch(type) {
						case InstructionType::MutateValue: {
							unsigned short leftValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short rightValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							
							this->gameState->mutateValueInstructions[id] = MutateValueInstruction(id, false, this->gameState);
							this->gameState->instructions[id] = &this->gameState->mutateValueInstructions[id];
							instructionParamsMap[id] = {leftValueID, rightValueID};
							i += 4;
							break;
						}
						case InstructionType::AwaitedRunCondition: {
							unsigned short evaluationValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short failSetID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short successSetID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
							
							this->gameState->runConditionInstructions[id] = RunConditionInstruction(id, true, this->gameState);
							this->gameState->instructions[id] = &this->gameState->runConditionInstructions[id];
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
							
							this->gameState->runConditionInstructions[id] = RunConditionInstruction(id, false, this->gameState);
							this->gameState->instructions[id] = &this->gameState->runConditionInstructions[id];
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
							
							this->gameState->drawPixelInstructions[id] = DrawPixelInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawPixelInstructions[id];
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
							
							this->gameState->drawLineInstructions[id] = DrawLineInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawLineInstructions[id];
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
							
							this->gameState->drawRectInstructions[id] = DrawRectInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawRectInstructions[id];
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
							
							this->gameState->fillRectInstructions[id] = FillRectInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->fillRectInstructions[id];

							instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID};
							i += 10;
							break;
						}
						case InstructionType::DrawCircle: {
							unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short radiusValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short xValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
							unsigned short yValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
							
							this->gameState->drawCircleInstructions[id] = DrawCircleInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawCircleInstructions[id];

							instructionParamsMap[id] = { colorValueID, radiusValueID, xValueID, yValueID};
							i += 8;
							break;
						}
						case InstructionType::FillCircle: {
							unsigned short colorValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short radiusValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short xValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
							unsigned short yValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);
							
							this->gameState->fillCircleInstructions[id] = FillCircleInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->fillCircleInstructions[id];

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
							
							this->gameState->drawTriangleInstructions[id] = DrawTriangleInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawTriangleInstructions[id];

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
							
							this->gameState->fillTriangleInstructions[id] = FillTriangleInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->fillTriangleInstructions[id];
							instructionParamsMap[id] = { colorValueID, x1ValueID, y1ValueID, x2ValueID, y2ValueID, x3ValueID, y3ValueID};
							i += 14;
							break;
						}
						case InstructionType::DrawImage: {
							unsigned short xValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short yValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short imageValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);


							this->gameState->drawImageInstructions[id] = DrawImageInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawImageInstructions[id];
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
							
							this->gameState->drawTextInstructions[id] = DrawTextInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->drawTextInstructions[id];
							instructionParamsMap[id] = { colorValueID, scaleValueID, textValueID, xValueID, yValueID};
							i += 10;
							break;
						}
						case InstructionType::Clear: {                
							this->gameState->clearInstructions[id] = ClearInstruction(id, false, this->displayRunner);
							this->gameState->instructions[id] = &this->gameState->clearInstructions[id];
							break;
						}
						case InstructionType::SetRotation: {
							unsigned short rotationValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							
							this->gameState->setRotationInstructions[id] = SetRotationInstruction(id, false, this->displayRunner, this->gameState);
							this->gameState->instructions[id] = &this->gameState->setRotationInstructions[id];

							instructionParamsMap[id] = { rotationValueID};
							i += 2;
							break;
						}
						case InstructionType::AwaitedRunSet: {
							unsigned short setID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							
							this->gameState->runSetInstructions[id] = RunSetInstruction(id, true, this->gameState);
							this->gameState->instructions[id] = &this->gameState->runSetInstructions[id];
							instructionParamsMap[id] = { setID};
							i += 2;
							break;
						}
						case InstructionType::RunSet: {
							unsigned short setID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							
							this->gameState->runSetInstructions[id] = RunSetInstruction(id, false, this->gameState);
							this->gameState->instructions[id] = &this->gameState->runSetInstructions[id];
							instructionParamsMap[id] = { setID};
							i += 2;
							break;
						}
						case InstructionType::DebugLog: {
							unsigned short logValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							
							this->gameState->debugLogInstructions[id] = DebugLogInstruction(id, false, this->gameState);
							this->gameState->instructions[id] = &this->gameState->debugLogInstructions[id];

							instructionParamsMap[id] = { logValueID};
							i += 2;
							break;
						}
						case InstructionType::Wait: {
							unsigned short amountValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);

							this->gameState->waitInstructions[id] = WaitInstruction(id, true, this->gameState);
							this->gameState->instructions[id] = &this->gameState->waitInstructions[id];

							instructionParamsMap[id] = { amountValueID };
							i += 2;
							break;
						}
						case InstructionType::Tone: {
							unsigned short speakerOutputValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short volumeValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short frequencyValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
							unsigned short durationValueID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);

							this->gameState->toneInstructions[id] = ToneInstruction(id, false, this->gameState, this->soundController);
							this->gameState->instructions[id] = &this->gameState->toneInstructions[id];

							instructionParamsMap[id] = {speakerOutputValueID, volumeValueID, frequencyValueID, durationValueID };
							i += 8;
							break;
						}
						case InstructionType::AwaitedTone: {
							unsigned short speakerOutputValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short volumeValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short frequencyValueID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short durationValueID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);

							this->gameState->toneInstructions[id] = ToneInstruction(id, true, this->gameState, this->soundController);
							this->gameState->instructions[id] = &this->gameState->toneInstructions[id];

							instructionParamsMap[id] = {speakerOutputValueID, volumeValueID, frequencyValueID, durationValueID };
							i += 8;
							break;
						}
						case InstructionType::InstructionSetType: {
							this->displayRunner->startLoading(1.0);
							unsigned short size = static_cast<unsigned short>(((data[i + 2] & 0b01111111) << 8) + data[i + 3]);
							bool async = static_cast<bool>((data[i + 2] >> 7) & 0b1);
							instructionSetParamsMap[id] = std::vector<unsigned short>(size);
							for(unsigned short vI = 0; vI < size * 2; vI += 2) {
								unsigned short instructionID = static_cast<unsigned short>((data[i + 4 + vI] << 8) + data[i + 5 + vI]);
								instructionSetParamsMap[id][vI / 2] = instructionID;
							}
							this->gameState->instructionSets[id] = InstructionSet(id, size, async, this->gameState);
							if (!setupSet) {
								this->gameState->setupInstructionSet = &this->gameState->instructionSets[id];
								setupSet = true;
							} else if (!mainSet) {
								this->gameState->mainInstructionSet = &this->gameState->instructionSets[id];
								mainSet = true;
							} else if (!renderSet) {
								this->gameState->renderInstructionSet = &this->gameState->instructionSets[id];
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
							this->gameState->numberValues[id] = NumberValue(id, f, 4);
							this->gameState->values[id] = &this->gameState->numberValues[id];
							i += 4;
							break;
						}
						case ValueType::pixelIndex: {
							unsigned short xID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short yID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							
							this->gameState->pixelValues[id] = PixelValue(id, this->gameState, this->displayRunner);
							this->gameState->values[id] = &this->gameState->pixelValues[id];
							valueParamsMap[id] = {xID, yID};
							i += 4;
							break;
						}
						case ValueType::image: {
							unsigned short dataID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short widthID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							unsigned short heightID = static_cast<unsigned short>((data[i + 6] << 8) + data[i + 7]);
							unsigned short keyColorID = static_cast<unsigned short>((data[i + 8] << 8) + data[i + 9]);

							this->gameState->imageValues[id] = ImageValue(id, this->gameState);
							this->gameState->values[id] = &this->gameState->imageValues[id];
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


							this->gameState->dataValues[id] = DataValue(id, size);
							this->gameState->values[id] = &this->gameState->dataValues[id];
							i += (2 + size);
							break;
						}
						case ValueType::digitalInputPointer: {
							unsigned short index = static_cast<unsigned short>(data[i + 2]);
							
							this->gameState->digitalInputValues[id] = DigitalInputValue(id, index, &this->mainRunner->digitalReader);
							this->gameState->values[id] = &this->gameState->digitalInputValues[id];
							i += 1;
							break;
						}
						case ValueType::analogInputPointer: {
							unsigned short index = static_cast<unsigned short>(data[i + 2]);
							
							this->gameState->analogInputValues[id] = AnalogInputValue(id, index, &this->mainRunner->analogReader);
							this->gameState->values[id] = &this->gameState->analogInputValues[id];
							i += 1;
							break;
						}
						case ValueType::speakerOutputPointer: {
							unsigned short index = static_cast<unsigned short>(data[i + 2]);
							
							this->gameState->speakerOutputValues[id] = SpeakerOutputValue(id, index, this->soundController);
							this->gameState->values[id] = &this->gameState->speakerOutputValues[id];
							i += 1;
							break;
						}
						case ValueType::systemPointer: {
							SystemConfigType configType = static_cast<SystemConfigType>(data[i + 2]);
							
							this->gameState->systemConfigValues[id] = SystemConfigValue(id, configType);
							this->gameState->values[id] = &this->gameState->systemConfigValues[id];
							i += 1;
							break;
						}

						case ValueType::listValue: {
							unsigned short listValueID = static_cast<unsigned short>((data[i + 2] << 8) + data[i + 3]);
							unsigned short listIndexID = static_cast<unsigned short>((data[i + 4] << 8) + data[i + 5]);
							
							this->gameState->listValues[id] = ListValue(id, this->gameState);
							this->gameState->values[id] = &this->gameState->listValues[id];
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
							
							this->gameState->textValues[id] = TextValue(id, size);
							this->gameState->values[id] = &this->gameState->textValues[id];

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
							this->gameState->listDeclarations[id] = ListDeclaration(id, size);
							this->gameState->values[id] = &this->gameState->listDeclarations[id];
							i += (2 + (size * 2));
							break;
						}
						case ValueType::evaluation: {
							EvaluationOperator evaluationOperator = static_cast<EvaluationOperator>(data[i + 2] >> 1);
							bool isStatic = static_cast<bool>(data[i + 2] & 0b1);
							unsigned short leftID = static_cast<unsigned short>((data[i + 3] << 8) + data[i + 4]);
							unsigned short rightID = static_cast<unsigned short>((data[i + 5] << 8) + data[i + 6]);

							this->gameState->evaluationValues[id] = EvaluationValue(
								id,
								evaluationOperator,
								isStatic,
								this->gameState
							);
							this->gameState->values[id] = &this->gameState->evaluationValues[id];
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
			if(this->gameState->values[x.first]->type != ValueType::listValue ) {
				this->gameState->values[x.first]->init(x.second);
			}
		}

		for (auto x : valueParamsMap) {
			if(this->gameState->values[x.first]->type == ValueType::listValue) {
				this->gameState->values[x.first]->init(x.second);
			}
		}

		for (auto x : instructionParamsMap) {
			this->gameState->instructions[x.first]->init(x.second);
		}

		for (auto x : instructionSetParamsMap) {
			this->gameState->instructionSets[x.first].init(x.second);
		}

		return true;
	}

};

#endif