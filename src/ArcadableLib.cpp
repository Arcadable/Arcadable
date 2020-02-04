#include "Arduino.h"
#include "ArcadableLib.h"
Arcadable *Arcadable::_instance = NULL;
Arcadable *Arcadable::getInstance() {
	if (!_instance)
    {
        _instance = new Arcadable;
    }

    return _instance;
}

void Arcadable::setup(
	SystemConfig *systemConfig,
	CRGB *pixels,
	GFXcanvas* canvas
) {
	this->systemConfig = systemConfig;
	this->pixels = pixels;
	this->canvas = canvas;
;
	for (unsigned short int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		this->pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}
	Wire.begin();
 	Wire.setClock(systemConfig->wireClock);
}

void Arcadable::step() {
	unsigned int currentMillis = millis();
	if(_pollImmediately || (currentMillis - _prevWireMillis > systemConfig->newGamePollingInterval)) {
		if (_pollImmediately) {
			_pollImmediately = false;
		}
		Wire.beginTransmission(systemConfig->eepromAddress);
		Wire.write(0);
		Wire.write(0);
		Wire.endTransmission();
		Wire.requestFrom(static_cast<unsigned int>(systemConfig->eepromAddress), 1);
		delay(1);
		if(_readyToLoad && Wire.available() == 1) {
			if(_gameLoaded) {
				_unloadGameLogic();
				_gameLoaded = false;
				_pollImmediately = true;
			} else {
				_readAndLoadGameLogic();
				_gameLoaded = true;
				_readyToLoad = false;
			}
		} else if(Wire.available() == 0) {
			_readyToLoad = true;
		}

		_prevWireMillis = millis();
	}
	if (_gameLoaded) {
		if (currentMillis - _prevGameMillis < systemConfig->minMillisPerFrame) {
			delay(systemConfig->minMillisPerFrame - (millis() - _prevGameMillis));
		}

		_doGameStep();
		_prevGameMillis = millis();
	} else {
		delay(systemConfig->minMillisPerFrame);
	}

}

void Arcadable::_doGameStep() {

	systemConfig->fetchInputValues();
	Serial.print("Step "); Serial.println(millis());

	for ( auto &condition : rootConditions ) {
		condition.execute();
	}


	if(systemConfig->layoutIsZigZag) {
		for(unsigned short int column = 0; column < systemConfig->screenWidth; column++) {
			if (column % 2 == 1) {
				CRGB tempLeds[systemConfig->screenHeight];
				for (int row = 0; row < systemConfig->screenHeight; row++) {
					tempLeds[(systemConfig->screenHeight - 1) - row] = pixels[column * systemConfig->screenWidth + row];  
				}
				for (int row = 0; row < systemConfig->screenHeight; row++) {
					pixels[column * systemConfig->screenWidth + row] = tempLeds[row];  
				}
			}
		}
	}
};

void Arcadable::_unloadGameLogic() {
	for (unsigned short int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}

	values.clear();
	lists.clear();
	calculations.clear();
	instructions.clear();
	conditions.clear();
}

void Arcadable::_readAndLoadGameLogic() {

	Serial.println("Start loading..");

	unsigned int currentParsePosition = 0;
	unsigned char valuesLengthData[2];
	_readEEPROM(currentParsePosition, 2, valuesLengthData);
	unsigned short int valuesLength = (valuesLengthData[0] << 8) + valuesLengthData[1];
	currentParsePosition += 2;
	unsigned char valuesData[valuesLength];
	_readEEPROM(currentParsePosition, valuesLength, valuesData);
	currentParsePosition += valuesLength;


	for (unsigned int i = 0 ; i < sizeof(valuesData)/sizeof(valuesData[0]) ;) {
		unsigned short id = static_cast<unsigned short>((valuesData[i + 0] << 8) + valuesData[i + 1]);
		u_int8_t raw[4] = {valuesData[i + 2], valuesData[i + 3], valuesData[i + 4], valuesData[i + 5]};
    float value = *(float *)raw;

		ValueType type = static_cast<ValueType>(valuesData[i + 6] >> 1);
		bool isPartOfList = static_cast<bool>(valuesData[i + 6] & 0b1);
		unsigned short listId = isPartOfList ? static_cast<unsigned short >((valuesData[i + 7] << 8) + valuesData[i + 8]) : 0;
		if ((valuesData[i + 2] >> 7) == 1) {
			value = value * -1;
		}

		Value typedValue(
			id,
			type,
			value,
			isPartOfList,
			listId
		);
		values.insert(std::pair<unsigned short int, Value>(id, typedValue));

		if (typedValue.isPartOfList) {
			lists.insert(std::pair<unsigned short int, Value>(listId, typedValue));
		}

		i += isPartOfList ? 9 : 7;
	}


	unsigned char calculationsLengthData[2];
	_readEEPROM(currentParsePosition, 2, calculationsLengthData);
	unsigned short int calculationsLength = (calculationsLengthData[0] << 8) + calculationsLengthData[1];
	currentParsePosition += 2;

	unsigned char calculationsData[calculationsLength];
	_readEEPROM(currentParsePosition, calculationsLength, calculationsData);
	currentParsePosition += calculationsLength;

	for (unsigned int i = 0 ; i < sizeof(calculationsData)/sizeof(calculationsData[0]) ;) {
		unsigned short id = static_cast<unsigned short>((calculationsData[i + 0] << 8) + calculationsData[i + 1]);
		unsigned short leftID = static_cast<unsigned short>((calculationsData[i + 2] << 8) + calculationsData[i + 3]);
		unsigned short rightID = static_cast<unsigned short>((calculationsData[i + 4] << 8) + calculationsData[i + 5]);
		CalculationOperator calculationOperator = static_cast<CalculationOperator>(calculationsData[i + 6] >> 3);
		bool rightIsValue = static_cast<bool>((calculationsData[i + 6] >> 2) & 0b1);
		bool leftIsValue = static_cast<bool>((calculationsData[i + 6] >> 1) & 0b1);
		bool isStatic = static_cast<bool>(calculationsData[i + 6] & 0b1);

		Calculation typedCalculation(
			id,
			leftIsValue,
			leftID,
			rightIsValue,
			rightID,
			calculationOperator,
			isStatic
		);
		calculations.insert(std::pair<unsigned short int, Calculation>(id, typedCalculation)); 

		i += 7;
	}

	unsigned char conditionsLengthData[2];
	_readEEPROM(currentParsePosition, 2, conditionsLengthData);
	unsigned short int conditionsLength = (conditionsLengthData[0] << 8) + conditionsLengthData[1];
	currentParsePosition += 2;

	unsigned char conditonsData[conditionsLength];
	_readEEPROM(currentParsePosition, conditionsLength, conditonsData);
	currentParsePosition += conditionsLength;

	for (unsigned int i = 0 ; i < sizeof(conditonsData)/sizeof(conditonsData[0]) ;) {
		unsigned short id = static_cast<unsigned short>((conditonsData[i + 0] << 8) + conditonsData[i + 1]);
		RelationalOperator conditionOperator = static_cast<RelationalOperator>(conditonsData[i + 2] >> 4);
		bool rootCondition = static_cast<bool>((conditonsData[i + 2] >> 3) & 0b1);
		bool hasFailedCondition = static_cast<bool>((conditonsData[i + 2] >> 2) & 0b1);
		bool leftIsValue = static_cast<bool>((conditonsData[i + 2] >> 1) & 0b1);
		bool rightIsValue = static_cast<bool>(conditonsData[i + 2] & 0b1);
		unsigned short leftID = static_cast<unsigned short>((conditonsData[i + 3] << 8) + conditonsData[i + 4]);
		unsigned short rightID = static_cast<unsigned short>((conditonsData[i + 5] << 8) + conditonsData[i + 6]);
		unsigned short conditionSuccessInstructionsID = static_cast<unsigned short>((conditonsData[i + 7] << 8) + conditonsData[i + 8]);
		unsigned short conditionFailedInstructionsID = hasFailedCondition ? static_cast<unsigned short>((conditonsData[i + 9] << 8) + conditonsData[i + 10]) : 0;

		Condition typedCondition(
			id,
			rootCondition,
			leftIsValue,
			leftID,
			rightIsValue,
			rightID,
			conditionOperator,
			conditionSuccessInstructionsID,
			hasFailedCondition,
			conditionFailedInstructionsID
		);
		if (rootCondition) {
			rootConditions.push_back(typedCondition);
		}
		conditions.insert(std::pair<unsigned short int, Condition>(id, typedCondition));

		i += hasFailedCondition ? 11 : 9;
	}

	unsigned char instructionsLengthData[2];
	_readEEPROM(currentParsePosition, 2, instructionsLengthData);
	unsigned short int instructionsLength = (instructionsLengthData[0] << 8) + instructionsLengthData[1];
	currentParsePosition += 2;

	unsigned char instructionsData[instructionsLength];
	_readEEPROM(currentParsePosition, instructionsLength, instructionsData);
	currentParsePosition += instructionsLength;

	for (unsigned int i = 0 ; i < sizeof(instructionsData)/sizeof(instructionsData[0]) ;) {
		unsigned short id = static_cast<unsigned short>((instructionsData[i + 0] << 8) + instructionsData[i + 1]);
		unsigned int leftID = static_cast<unsigned int>((instructionsData[i + 2] << 24) + (instructionsData[i + 3] << 16) + (instructionsData[i + 4] << 8) + instructionsData[i + 5]);
		unsigned int rightID = static_cast<unsigned int>((instructionsData[i + 6] << 24) + (instructionsData[i + 7] << 16) + (instructionsData[i + 8] << 8) + instructionsData[i + 9]);
		InstructionType instructionType = static_cast<InstructionType>(instructionsData[i + 10] >> 1);
		bool rightIsValue = static_cast<bool>(instructionsData[i + 10] & 0b1);
		Instruction typedInstruction(
			id,
			leftID,
			rightIsValue,
			rightID,
			instructionType
		);
		instructions.insert(std::pair<unsigned short int, Instruction>(id, typedInstruction));
		i += 11;
	}
	Serial.println("Done loading!");
}

void Arcadable::_readEEPROM(unsigned int startAddress, unsigned int dataLength, unsigned char *dataReceiver) {
  unsigned int currentAddress = startAddress;
  unsigned int currentReadIndex = 0;
  while (currentReadIndex < dataLength) {
    Wire.beginTransmission(systemConfig->eepromAddress);
    Wire.write(currentAddress >> 8);
    Wire.write(currentAddress & 0xFF);
    Wire.endTransmission();
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