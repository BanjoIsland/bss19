#include <Arduino.h>
#include "quadencoder.h"

int QEM [16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Quadrature Encoder Matrix
int prev = 0;
int now = 0;
int count;
bool debug_encoder;

void encoder_setup(bool mode) {
  debug_encoder = mode;
  pinMode(QBT, INPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
}

int read_encoder() {
  if (digitalRead(QBT)) {
    prev = now;
    now = digitalRead(CLK) * 2 + digitalRead(DT); // Convert binary input to decimal value
    count += QEM[prev * 4 + now];
  }  else {
    count = 0;
  }
  return count;
}
