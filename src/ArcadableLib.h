/*
	ArcadableLib.h - Library for running games on LED strips.
	Created by Niek de Wit, January, 2020.
	Released into the public domain.
*/
#ifndef ArcadableLib_h
#define ArcadableLib_h

#include "Arduino.h"
#include <map>
#include <vector>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>

class Value;
class Calculation;
class Condition;
class Instruction;
class SystemConfig;
class GFXcanvas;

class Arcadable {
	public:
		SystemConfig *systemConfig;

		std::map<unsigned short int, Value> values;
		std::map<unsigned short int, Calculation> calculations;
		std::map<unsigned short int, Condition> conditions;
		std::multimap<unsigned short int, Instruction> instructions;
		std::multimap<unsigned short int, Value> lists;
		std::vector<Condition> rootConditions;
		CRGB* pixels;
		CRGB* pixelsBuffer;
		GFXcanvas* canvas;

		long int startMillis;

		static Arcadable *getInstance();
		void setup(
			SystemConfig *systemConfig,
			CRGB *pixels,
			CRGB *pixelsBuffer,
			GFXcanvas* canvas
		);

		void step();

	private:
		static Arcadable *_instance;
		unsigned int _prevGameMillis;
		unsigned int _prevWireMillis;
		bool _gameLoaded = false;
		bool _readyToLoad = true;
		bool _pollImmediately = false;

		void _doGameStep();
		void _unloadGameLogic();
		void _readAndLoadGameLogic();
		void _readEEPROM(
			unsigned int startAddress,
			unsigned int dataLength,
			unsigned char *dataReceiver
		);

};
#include "SystemConfig.h"
#include "Value.h"
#include "Calculation.h"
#include "Instruction.h"
#include "Condition.h"
#include <FastLED_GFX.h>

#endif