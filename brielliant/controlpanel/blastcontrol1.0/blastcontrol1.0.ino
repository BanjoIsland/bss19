/*
  Gametech, pilot demolitions officer controller
  Author: Ryan Williams
  Revised: 5.12.2019
*/

#include "humidity.h"
#include "buttons.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x01;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, SUCCESS};

bool debug_mode = false;

void run_mode();
void read_serial();

volatile enum States state = IDLING;

void setup() {
  Serial.begin(9600);
  humSetup();
  butSetup();
  ledSetup();
}


void loop() {
  run_mode();
  delay(10);
}

void run_mode() {
  read_serial();
  switch (state)
  {
    case IDLING:
      break;
    case TRYHARD_A:       // humidity check
      if (humCheck()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_B:       // button check
      if (butCheck()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_C:       // switch check
      if (swCheck()) {
        state = SUCCESS;   
      }
      break;
    case SUCCESS:
      Serial.write((byte) 0xFF);
      if (debug_mode) Serial.println("I won");
      state = IDLING;
      setFlashCount(3);
      ledSetState(LED_FLASH_GREEN);
      break;
      
  }
}

// Checks the serial port for a command from the main terminal
void read_serial() {
  uint8_t incoming_byte = 0xAA; //Unused value to prevent false flags
  if (Serial.available() > 0) {
    if (debug_mode) {
      incoming_byte = Serial.parseInt();
    } else {
      incoming_byte = Serial.read();
    }
  }
  switch (incoming_byte)
  {
    case 0x00:        // State request; return 0x00:idling, 0x01:TRYING HARD
      if (state == IDLING) {
        if (debug_mode) Serial.println("I'm not trying");
        Serial.write((byte)0x01);       //I'm idling
      } else {
        if (debug_mode) Serial.println("I'm trying");
        Serial.write((byte)0x00);       //I'm trying hard
      }
      break;
    case 0x01:        //Identify; return console ID
      Serial.write((byte)CONSOLE_ID);
      if (debug_mode) Serial.println("You requested my ID");
      break;
    case 0x09:        //Reset
      state = IDLING;             //TODO: Think about possible consequences of this
      break;
    case 0x10:
      if (debug_mode) Serial.println("going to case A");
      state = TRYHARD_A;
      break;
    case 0x11:
      if (debug_mode) Serial.println("going to case B");
      state = TRYHARD_B;
      break;
    case 0x12:
      if (debug_mode) Serial.println("going to case C");
      state = TRYHARD_C;
      break;
  }
}
