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
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <Wire.h>


#include "SystemConfig.h"
#include "values/Value.h"
#include "instructions/Instruction.h"
#include "instructions/InstructionSet.h"
#include "fastledGFX/FastLED_GFX.h"

#include "values/NumberValue.h"
#include "values/TextValue.h"
#include "values/SystemConfigValue.h"
#include "values/PixelValue.h"
#include "values/ListValue.h"
#include "values/ListDeclaration.h"
#include "values/EvaluationValue.h"
#include "values/DigitalInputValue.h"
#include "values/AnalogInputValue.h"

#include "instructions/MutateValueInstruction.h"
#include "instructions/RunConditionInstruction.h"
#include "instructions/RunSetInstruction.h"
#include "instructions/SetRotationInstruction.h"
#include "instructions/FillTriangleInstruction.h"
#include "instructions/FillRectInstruction.h"
#include "instructions/FillCircleInstruction.h"
#include "instructions/DrawCircleInstruction.h"
#include "instructions/DrawLineInstruction.h"
#include "instructions/DrawPixelInstruction.h"
#include "instructions/DrawRectInstruction.h"
#include "instructions/DrawTextInstruction.h"
#include "instructions/DrawTriangleInstruction.h"
#include "instructions/DebugLogInstruction.h"
#include "instructions/ClearInstruction.h"

class Arcadable {
	public:
		SystemConfig *systemConfig;

		std::map<unsigned short int, NumberValue> numberValues;
		std::map<unsigned short int, PixelValue> pixelValues;
		std::map<unsigned short int, DigitalInputValue> digitalInputValues;
		std::map<unsigned short int, AnalogInputValue> analogInputValues;
		std::map<unsigned short int, SystemConfigValue> systemConfigValues;
		std::map<unsigned short int, ListDeclaration> listDeclarations;
		std::map<unsigned short int, ListValue> listValues;
		std::map<unsigned short int, TextValue> textValues;
		std::map<unsigned short int, EvaluationValue> evaluationValues;

		std::map<unsigned short int, MutateValueInstruction> mutateValueInstructions;
		std::map<unsigned short int, RunConditionInstruction> runConditionInstructions;
		std::map<unsigned short int, DrawPixelInstruction> drawPixelInstructions;
		std::map<unsigned short int, DrawLineInstruction> drawLineInstructions;
		std::map<unsigned short int, DrawRectInstruction> drawRectInstructions;
		std::map<unsigned short int, FillRectInstruction> fillRectInstructions;
		std::map<unsigned short int, DrawCircleInstruction> drawCircleInstructions;
		std::map<unsigned short int, FillCircleInstruction> fillCircleInstructions;
		std::map<unsigned short int, DrawTriangleInstruction> drawTriangleInstructions;
		std::map<unsigned short int, FillTriangleInstruction> fillTriangleInstructions;
		std::map<unsigned short int, DrawTextInstruction> drawTextInstructions;
		std::map<unsigned short int, ClearInstruction> clearInstructions;
		std::map<unsigned short int, SetRotationInstruction> setRotationInstructions;
		std::map<unsigned short int, RunSetInstruction> runSetInstructions;
		std::map<unsigned short int, DebugLogInstruction> debugLogInstructions;

		std::map<unsigned short int, Value*> values;
		std::map<unsigned short int, Instruction*> instructions;
        std::map<unsigned short int, InstructionSet> instructionSets;
		InstructionSet* mainInstructionSet;
        InstructionSet* renderInstructionSet;
		CRGB* pixels;
		CRGB* pixelsBuffer;
		GFXcanvas* canvas;

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
		unsigned int _prevMainMillis;
        unsigned int _prevRenderMillis;
		unsigned int _prevWireMillis;
		bool _gameLoaded = false;
		bool _readyToLoad = true;
		bool _pollImmediately = false;
		bool _refresh = false;

		void _mainStep();
		void _renderStep();
		void _unloadGameLogic();
		void _readAndLoadGameLogic();
		void _readEEPROM(
			unsigned int startAddress,
			unsigned int dataLength,
			unsigned char *dataReceiver
		);

};

#endif