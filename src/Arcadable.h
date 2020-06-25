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
class Value;
class Instruction;
class InstructionSet;
class SystemConfig;
class GFXcanvas;

class Arcadable {
	public:
		SystemConfig *systemConfig;

		std::map<unsigned short int, Value> values;
		std::map<unsigned short int, Instruction> instructions;
        std::map<unsigned short int, InstructionSet> instructionSets;
		unsigned short int mainInstructionSet;
        unsigned short int renderInstructionSet;
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

		void mainStep();
		void renderStep();

	private:
		static Arcadable *_instance;
		unsigned int _prevMainMillis;
        unsigned int _prevRenderMillis;
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
#include "LogicElement.h"
#include "Value.h"
#include "ValueArrayValueType.h"
#include "TextValue.h"
#include "SystemConfigValue.h"
#include "PixelValue.h"
#include "NumberValueType.h"
#include "NumberValue.h"
#include "ListValue.h"
#include "ListDeclaration.h"
#include "EvaluationValue.h"
#include "DigitalInputValue.h"
#include "AnalogInputValue.h"

#include "Instruction.h"
#include "InstructionSet.h"
#include "MutateValueInstruction.h"
#include "RunConditionInstruction.h"
#include "RunSetInstruction.h"
#include "SetRotationInstruction.h"
#include "FillTriangleInstruction.h"
#include "FillRectInstruction.h"
#include "FillCircleInstruction.h"
#include "DrawCircleInstruction.h"
#include "DrawLineInstruction.h"
#include "DrawPixelInstruction.h"
#include "DrawRectInstruction.h"
#include "DrawTextInstruction.h"
#include "DrawTriangleInstruction.h"
#include "DebugLogInstruction.h"
#include "ClearInstruction.h"
#include "InstructionSet.h"

#include <FastLED_GFX.h>

#endif