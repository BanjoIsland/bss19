#include <Arduino.h>
#include "quadencoder.h"

uint8_t QEM [16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Quadrature Encoder Matrix
uint8_t prev = 0;
uint8_t now = 0;
int count = 0;
int target_count = 0xFFFF;
bool debug_encoder;

void encoder_setup(bool mode) {
  debug_encoder = mode;
  pinMode(QBT, INPUT_PULLUP);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
}

void set_encoder_count(uint16_t val) {
  target_count = val * 4;
}

bool encoder_check() {
  read_encoder();
  if (count == target_count) {
    count = 0;
    prev = count;
    now = prev;
    return true;
  }
  return false;
}

void read_encoder() {
  if (digitalRead(QBT)) {
    prev = now;
    now = digitalRead(CLK) * 2 + digitalRead(DT); // Convert binary input to decimal value
    count += QEM[prev * 4 + now];
    if (debug_encoder) Serial.println("Encoder value: " + String(count));
  }  else {
    count = 0;
    prev = 0;
    now = 0;
  }
  return count;
}
