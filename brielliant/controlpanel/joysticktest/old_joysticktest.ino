#include "joystick.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  joySetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  joyCheck();
  delay(10);
}
