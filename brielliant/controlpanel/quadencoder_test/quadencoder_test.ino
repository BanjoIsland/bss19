#include "quadencoder.h"

void setup() {
  pinMode(QBT, INPUT);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(read_encoder());
}
