#include "Arduino.h"
#include "buttons.h"

void butSetup() {
  pinMode(BLUE_PIN, INPUT_PULLUP);
  pinMode(YELLOW_PIN, INPUT_PULLUP); //curd rockets
  pinMode(GREEN_PIN, INPUT_PULLUP);
  pinMode(RED_PIN, INPUT_PULLUP);

  pinMode(SW1_PIN, INPUT); //curd primer
  pinMode(SW2_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);
}

bool butAllCheck() {
  int blueState = digitalRead(BLUE_PIN);
  int yellowState = digitalRead(YELLOW_PIN);
  int greenState = digitalRead(GREEN_PIN);
  int redState = digitalRead(RED_PIN);
  
  if (blueState==LOW && yellowState==LOW &&
    greenState==LOW && redState==LOW) {
    return true;  
  } else {
    return false;
  }
}

bool redCheck() {
  int redState = digitalRead(RED_PIN);
  if (redState==LOW) {
    return true;
  }
  return false;
}
bool greenCheck() {
  int greenState = digitalRead(GREEN_PIN);
  if (greenState==LOW) {
    return true;
  }
  return false;
}
bool blueCheck() {
  int blueState = digitalRead(BLUE_PIN);
  if (blueState==LOW) {
    return true;
  }
  return false;
}
bool yellowCheck() {
  int yellowState = digitalRead(YELLOW_PIN);
  if (yellowState==LOW) {
    return true;
  }
  return false;
}

bool swAllHighCheck() {
  int sw1State = digitalRead(SW1_PIN);
  int sw2State = digitalRead(SW2_PIN);
  int sw3State = digitalRead(SW3_PIN);
  
  if (sw1State==HIGH && sw2State==HIGH && sw3State==HIGH) {
    return true;  
  } else {
    return false;
  }  
}

bool swAllLowCheck() {
  int sw1State = digitalRead(SW1_PIN);
  int sw2State = digitalRead(SW2_PIN);
  int sw3State = digitalRead(SW3_PIN);
  
  if (sw1State==LOW && sw2State==LOW && sw3State==LOW) {
    return true;  
  } else {
    return false;
  }  
}

int curd_primer() {
  return digitalRead(SW1_PIN);
}

int lactation() {
  return !digitalRead(YELLOW_PIN);
}

int fondue_heater() {
  return digitalRead(SW3_PIN);
}

int acid_attack() {
  return !digitalRead(GREEN_PIN);
}

int enzymatic() {
  return !digitalRead(SW2_PIN);
}
