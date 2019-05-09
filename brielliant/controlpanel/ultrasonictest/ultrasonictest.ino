#include <Arduino.h>
#include "ultrasonic.h"

void setup() {
  Serial.begin(9600);
  ultraSetup();
}

void loop() {
  if (ultraCheck()) {
    Serial.println("object detected!");
  }
  delay(1);
}
