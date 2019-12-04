#include "Arduino.h"
#include "Arcadable.h"

void Game::setConfiguration(
	InputPins *inputs,
	SystemConfig *systemConfig,
	CRGB* pixels
) {
	this->inputs = inputs;
	this->systemConfig = systemConfig;
	this->pixels = pixels;

	for(int inputPin : inputs->expandedProperties) {
		pinMode(inputPin, INPUT);
	}

	for (int i = 0; i < systemConfig->screenWidth * systemConfig->screenHeight; i++) {
		pixels[i] = CRGB(0 + (0 << 8) + (0 << 16));
	}
}

void Game::setGameLogic(
	std::vector<std::vector<int>> *untypedConditions,
	std::vector<std::vector<int>> *untypedCalculations,
	std::vector<std::vector<int>> *untypedValues,
	std::vector<std::vector<int>> *untypedInstructions,
	std::vector<int> * untypedGamestate
) {

	for ( auto &condition : *untypedConditions ) {
		Condition typedCondition(
			condition[0],
			condition[1],
			condition[2],
			condition[3],
			static_cast<RelationalOperator>(condition[4]),
			condition[5],
			condition[6],
			condition[7]
		);
		conditions.insert(std::pair<int, Condition>(condition[0], typedCondition));
	}
	for ( auto &calculation : *untypedCalculations ) {
		Calculation typedCalculation(
			calculation[0],
			calculation[1],
			calculation[2],
			calculation[3],
			static_cast<CalculationOperator>(calculation[4])
		);
		calculations.insert(std::pair<int, Calculation>(calculation[0], typedCalculation)); 
	}
	for ( auto &value : *untypedValues ) {
		Value typedValue(
			value[0],
			static_cast<ValueType>(value[1]),
			value[2]
		);
		values.insert(std::pair<int, Value>(value[0], typedValue));
	}
	for ( auto &instruction : *untypedInstructions ) {
		Instruction typedInstruction(
			instruction[0],
			instruction[1],
			instruction[2]
		);
		instructions.insert(std::pair<int, Instruction>(instruction[0], typedInstruction));
	}
	int i = 0;
	for ( auto &state : *untypedGamestate ) {
		gamestate.insert(std::pair<int, int>(i, state));
		i++;
	}
}
void Game::step() {
	int currentMillis = millis();
	if (currentMillis - _prevMillis < systemConfig->minMillisPerFrame) {
		delay(systemConfig->minMillisPerFrame - (millis() - _prevMillis));
	}
	_doGameStep();
	_prevMillis = millis();
}

void Game::_doGameStep() {
	inputs->fetchInputValues();


	for (auto const& condition : conditions) {
		if (condition.second.rootCondition == true) {
			condition.second.execute(this);
		}
	}
}
