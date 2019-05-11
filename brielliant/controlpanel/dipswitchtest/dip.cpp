#include "Arduino.h"
#include "dip.h"

unsigned int dipReading = 0;

void dipSetup() {
  for (int i=0; i<numDipPins; i++) {
    pinMode(dipPins[i], INPUT_PULLUP);
  }
}

void dipRead() {
  
  uint16_t pinRead = 0;
  for (uint16_t x = 0; x < numDipPins; x++)
  {
    uint16_t pinRead = digitalRead(dipPins[x]);
    dipReading = dipReading + (pinRead << x);
  }
  Serial.println(dipReading, BIN);
  dipReading = 0;
  
  /*
  for (uint16_t x = 0; x < numDipPins; x++) {
    Serial.print(digitalRead(dipPins[x]));
  }
  Serial.println("");
  */
}

