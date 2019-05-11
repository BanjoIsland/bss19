#include "dip.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dipSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  dipRead();
  delay(1000);
}
