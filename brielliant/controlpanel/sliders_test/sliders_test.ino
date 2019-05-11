#include "sliders.h"

const uint8_t CONSOLE_ID = 0x00;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, SUCCESS};
enum Sensors {NONE, B_A, B_B, BOTH};

bool debug_mode = false;


void setup() {
  Serial.begin(9600);
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
}

void loop() {
  Serial.println("L = " + String(left_level_check()));
  Serial.println("R = " + String(right_level_check()));
  delay(500);
}
