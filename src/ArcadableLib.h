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

class Value;
class Calculation;
class Condition;
class Instruction;
class SystemConfig;

class Game {
	public:
		SystemConfig *systemConfig;
		std::map<int, Value> values;
		std::map<int, Calculation> calculations;
		std::map<int, Condition> conditions;
		std::multimap<int, Instruction> instructions;
		std::multimap<int, Value> lists;
		CRGB* pixels;
		static Game *getInstance();
		void setConfiguration(
			SystemConfig *systemConfig,
			CRGB *pixels
		);
		void setGameLogic(
			std::vector<std::vector<int>> *untypedConditions,
			std::vector<std::vector<int>> *untypedCalculations,
			std::vector<std::vector<int>> *untypedValues,
			std::vector<std::vector<int>> *untypedInstructions
		);
		void step();

	private:
		static Game *_instance;
		unsigned int _prevMillis;
		void _doGameStep();

};
#include "SystemConfig.h"
#include "Value.h"
#include "Calculation.h"
#include "Instruction.h"
#include "Condition.h"


#endif