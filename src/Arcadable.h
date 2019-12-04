/*
	Arcadable.h - Library for running games on LED strips.
	Created by Niek de Wit, January, 2020.
	Released into the public domain.
*/
#ifndef Arcadable_h
#define Arcadable_h

#include "Arduino.h"
#include <map>
#include <vector>
#include "InputPins.h"
#include "SystemConfig.h"
#define FASTLED_INTERNAL
#include <FastLED.h>

class Value;
class Calculation;
class Condition;
class Instruction;

class Game {
	public:
		InputPins *inputs;
		SystemConfig *systemConfig;
		std::map<int, Value> values;
		std::map<int, Calculation> calculations;
		std::map<int, Condition> conditions;
		std::multimap<int, Instruction> instructions;
		std::map<int, int> gamestate;
		CRGB* pixels;
		void setConfiguration(
			InputPins *inputs,
			SystemConfig *systemConfig,
			CRGB* pixels
		);
		void setGameLogic(
			std::vector<std::vector<int>> *untypedConditions,
			std::vector<std::vector<int>> *untypedCalculations,
			std::vector<std::vector<int>> *untypedValues,
			std::vector<std::vector<int>> *untypedInstructions,
			std::vector<int> *untypedGamestate
		);
		void step();

	private:
		unsigned int _prevMillis;
		void _doGameStep();
};

#include "Value.h"
#include "Calculation.h"
#include "Instruction.h"
#include "Condition.h"


#endif