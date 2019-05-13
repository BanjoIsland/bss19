#include "Arduino.h"
#include "buttons.h"

void butSetup() {
  pinMode(BLUE_PIN, INPUT_PULLUP);
  pinMode(YELLOW_PIN, INPUT_PULLUP);
  pinMode(WHITE_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, INPUT_PULLUP);

  pinMode(SW1_PIN, INPUT);
  pinMode(SW2_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);
}

bool butCheck() {
  int blueState = digitalRead(BLUE_PIN);
  int yellowState = digitalRead(YELLOW_PIN);
  int whiteState = digitalRead(WHITE_PIN);
  int redState = digitalRead(RED_PIN);
  
  if (blueState==LOW && yellowState==LOW &&
    whiteState==LOW && redState==LOW) {
    return true;  
  } else {
    return false;
  }
}

bool swCheck() {
  int sw1State = digitalRead(SW1_PIN);
  int sw2State = digitalRead(SW2_PIN);
  int sw3State = digitalRead(SW3_PIN);
  
  if (sw1State==HIGH && sw2State==HIGH && sw3State==HIGH) {
    return true;  
  } else {
    return false;
  }  
}

