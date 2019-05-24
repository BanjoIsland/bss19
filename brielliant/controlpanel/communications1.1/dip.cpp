#include "Arduino.h"
#include "dip.h"

uint16_t target_dip_state;
bool debug_pin = false;

uint16_t smiley = 11; //dont care about this
uint16_t dip_states[] = {0, 3, 7, smiley, 1023};

void dip_setup(bool mode) {
  debug_pin = mode;
  for (int i = 0; i < NUM_PINS; i++) {
    pinMode(dip_pins[i], INPUT_PULLUP);
  }
}

void set_dip_target(uint16_t state) {
  target_dip_state = dip_states[state];
}

bool check_dip() {
  if (read_dip() == (uint16_t) target_dip_state) {
    return true;
  }
  return false;
}

bool check_smiley() {
  uint16_t dip = read_dip();
  if ((dip & 0x78) == 0x78 && (~dip & 0x303) == 0x303) { // = 0b0011111100 -> 1100000011
    return true;
  }
  return false;
}

uint16_t read_dip() {
  uint8_t pin_read = 0;
  uint16_t dip_read = 0;
  for (uint16_t x = 0; x < NUM_PINS; x++) {
    uint8_t pin_read = digitalRead(dip_pins[x]);
    dip_read = dip_read + (pin_read << x);
  }
//  if (debug_pin) Serial.println(target_dip_state, BIN);
//  if (debug_pin) Serial.println(dip_read, BIN);
  return dip_read;
  
  /*
  for (uint16_t x = 0; x < NUM_PINS ; x++) {
  }
  Serial.println("");
  */
}
