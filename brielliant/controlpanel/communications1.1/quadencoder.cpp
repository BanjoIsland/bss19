#include <Arduino.h>
#include "quadencoder.h"

int QEM [16] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Quadrature Encoder Matrix
int prev = 0;
int now = 0;
int count = 0;
int target_count = 0xFFFF;
bool debug_encoder;

void encoder_setup(bool mode) {
  debug_encoder = mode;
  //pinMode(QBT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  prev = 0;
  now = 0;
  count = 0;
}

void set_encoder_count(uint16_t val) {
  target_count = val;
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
  //if (digitalRead(QBT)) {
    prev = now;
    now = digitalRead(CLK) * 2 + digitalRead(DT); // Convert binary input to decimal value
    count += QEM[prev * 4 + now];
    if (debug_encoder) Serial.println("now value: " + String(now));
    if (debug_encoder) Serial.println("count value: " + String(count));
  //}  else {
    //count = 0;
    //prev = 0;
    //now = 0;
  //}
  return count;
}

bool encRotateRightCheck() {
  encReadCount();
  if (count >= target_count) {
    count = 0;
    prev = count;
    now = prev;
    return true;
  }
  return false;
}

bool encRotateLeftCheck() {
  encReadCount();
  if (debug_encoder) Serial.println("encoder: " + String(count));
  if (count <= target_count) {
    count = 0;
    prev = count;
    now = prev;
    return true;
  }
  return false;
}

void encReadCount() { 
   now = digitalRead(CLK); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (now != prev){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(DT) != now) { 
       count ++;
     } else {
       count --;
     }
     if (debug_encoder) Serial.print("Position: ");
     if (debug_encoder) Serial.println(count);
   } 
   prev = now; // Updates the previous state of the outputA with the current state
 }
