/*
  Gametech, pilot navigator controller
  Author: Ryan Williams
  Revised: 5.12.2019
*/

#include "joystick.h"
#include "humidity.h"
#include "sliders.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x00;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, TRYHARD_D, SUCCESS};

bool debug_mode = false;

void run_mode();
void read_serial();

volatile enum States state = IDLING;

void setup() {
  Serial.begin(9600);
  joySetup(debug_mode);
  humSetup(debug_mode);
  slider_setup(debug_mode);
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
    case TRYHARD_A:       /// humidity check needs basepoint set in read_serial
      if (debug_mode) Serial.println("Humidity state");
      if (humCheck()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_B:       // Joystick check jerky
      if (debug_mode) Serial.println("Joystick state");
      if (joyUpdate()) {
        state = SUCCESS;
      }
    case TRYHARD_C:       // Joystick check up
      if (debug_mode) Serial.println("Joystick state");
      if (joyUpdate()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_D:       // Slider check
      if (debug_mode) Serial.println("Slider state");
      slider_set_sequence(1);
      if (slider_check()) {
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
      if (debug_mode) Serial.println("The drill is overheating / humidity");
      humSetBasePt();
      state = TRYHARD_A;
      break;
    case 0x11:
      if (debug_mode) Serial.println("Hit hard cheese / Joystick LRLRLR");
      joySetSequence(0);
      state = TRYHARD_C;
      break;
    case 0x12:
      if (debug_mode) Serial.println("Hit hard cheese / Joystick LRLRLR");
      joySetSequence(1);
      state = TRYHARD_B;
      break;
  }
}
