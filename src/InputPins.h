#pragma once
#define InputPins_h
class InputPins {
    public:
        int inputUpPin;
        int inputRightPin;
        int inputDownPin;
        int inputLeftPin;
        int expandedProperties[4];
        bool inputValues[4];
        InputPins(
            int inputUpPin,
            int inputRightPin,
            int inputDownPin,
            int inputLeftPin
        );
        void fetchInputValues();
};
