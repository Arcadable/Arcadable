#include <Arduino.h>
#include "./src/arcadableController.h"

ArcadableController controller;

void setup () { 
    Serial.begin(9600);
    controller.init();
}
void loop () { }
