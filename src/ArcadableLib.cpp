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
	CRGB *pixels
) {
	this->systemConfig = systemConfig;
	this->pixels = pixels;

	for (unsigned int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}

	Wire.begin();
 	Wire.setClock(systemConfig->wireClock);
}

void Arcadable::step() {

	int currentMillis = millis();
	if(_pollImmediately || (currentMillis - _prevWireMillis > systemConfig->newGamePollingInterval)) {
		if (_pollImmediately) {
			_pollImmediately = false;
		}
		Serial.println("Check for game..");
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
	
	for (auto &condition : conditions) {
		if (condition.second.rootCondition == true) {
			condition.second.execute();
		}
	}
};

void Arcadable::_unloadGameLogic() {
	for (unsigned int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
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
	unsigned char valuesData[valuesLength * 9];
	_readEEPROM(currentParsePosition, valuesLength * 9, valuesData);
	currentParsePosition += valuesLength * 9;


	for (unsigned int i = 0 ; i < sizeof(valuesData)/sizeof(valuesData[0]) ; i += 9) {
		unsigned short id = static_cast<unsigned short>((valuesData[i + 0] << 8) + valuesData[i + 1]);
		unsigned short listId = static_cast<unsigned short >((valuesData[i + 7] << 8) + valuesData[i + 8]);
		Value typedValue(
			id, // ID
			static_cast<ValueType>(valuesData[i + 6] >> 1), // Type
			static_cast<unsigned int>((valuesData[i + 2] << 24) + (valuesData[i + 3] << 16) + (valuesData[i + 4] << 8) + valuesData[i + 5]), // Value
			static_cast<bool>(valuesData[i + 6] & 0b1), // isPartOfList
			listId // ListID
		);
		values.insert(std::pair<int, Value>(id, typedValue));

		if (typedValue.isPartOfList) {
			lists.insert(std::pair<int, Value>(listId, typedValue));
		}
	}

	unsigned char calculationsLengthData[2];
	_readEEPROM(currentParsePosition, 2, calculationsLengthData);
	unsigned short int calculationsLength = (calculationsLengthData[0] << 8) + calculationsLengthData[1];
	currentParsePosition += 2;

	unsigned char calculationsData[calculationsLength * 7];
	_readEEPROM(currentParsePosition, calculationsLength * 7, calculationsData);
	currentParsePosition += calculationsLength * 7;

	for (unsigned int i = 0 ; i < sizeof(calculationsData)/sizeof(calculationsData[0]) ; i += 7) {
		unsigned short id = static_cast<unsigned short>((calculationsData[i + 0] << 8) + calculationsData[i + 1]);
		Calculation typedCalculation(
			id, // ID
			static_cast<bool>(calculationsData[i + 6] & 0b1), //ending
			static_cast<unsigned short>((calculationsData[i + 2] << 8) + calculationsData[i + 3]), // calculationLeftID
			static_cast<unsigned short>((calculationsData[i + 4] << 8) + calculationsData[i + 5]), // calculationRightID
			static_cast<CalculationOperator>(calculationsData[i + 6] >> 1) // calculationOperator
		);
		calculations.insert(std::pair<int, Calculation>(id, typedCalculation)); 
	}
		
	unsigned char conditionsLengthData[2];
	_readEEPROM(currentParsePosition, 2, conditionsLengthData);
	unsigned short int conditionsLength = (conditionsLengthData[0] << 8) + conditionsLengthData[1];
	currentParsePosition += 2;

	unsigned char conditonsData[conditionsLength * 11];
	_readEEPROM(currentParsePosition, conditionsLength * 11, conditonsData);
	currentParsePosition += conditionsLength * 11;

	for (unsigned int i = 0 ; i < sizeof(conditonsData)/sizeof(conditonsData[0]) ; i += 11) {
		unsigned short id = static_cast<unsigned short>((conditonsData[i + 0] << 8) + conditonsData[i + 1]);
		Condition typedCondition(
			id, // ID
			static_cast<bool>((conditonsData[i + 2] >> 1) & 0b1), // rootCondition
			static_cast<unsigned short>((conditonsData[i + 3] << 8) + conditonsData[i + 4]), // conditionCalculationLeftID
			static_cast<unsigned short>((conditonsData[i + 5] << 8) + conditonsData[i + 6]), // conditionCalculationRightID
			static_cast<RelationalOperator>(conditonsData[i + 2] >> 2), // conditionOperator
			static_cast<unsigned short>((conditonsData[i + 7] << 8) + conditonsData[i + 8]), // conditionSuccesInstructionsID
			static_cast<bool>(conditonsData[i + 2] & 0b1), // hasFailedCondition
			static_cast<unsigned short>((conditonsData[i + 9] << 8) + conditonsData[i + 10]) // conditionFailedInstructionsID
		);
		conditions.insert(std::pair<int, Condition>(id, typedCondition));
	}

	unsigned char instructionsLengthData[2];
	_readEEPROM(currentParsePosition, 2, instructionsLengthData);
	unsigned short int instructionsLength = (instructionsLengthData[0] << 8) + instructionsLengthData[1];
	currentParsePosition += 2;

	unsigned char instructionsData[instructionsLength * 6];
	_readEEPROM(currentParsePosition, instructionsLength * 6, instructionsData);
	currentParsePosition += instructionsLength * 6;

	for (unsigned int i = 0 ; i < sizeof(instructionsData)/sizeof(instructionsData[0]) ; i += 6) {
		unsigned short id = static_cast<unsigned short>((instructionsData[i + 0] << 8) + instructionsData[i + 1]);
		Instruction typedInstruction(
			id,
			static_cast<unsigned short>((instructionsData[i + 2] << 8) + instructionsData[i + 3]), // valueLeftID
			static_cast<unsigned short>((instructionsData[i + 4] << 8) + instructionsData[i + 5]) // calculationRightID
		);
		instructions.insert(std::pair<int, Instruction>(id, typedInstruction));
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