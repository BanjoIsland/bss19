/*
  Gametech, Navigation Officer controller v1.1
  Author: Ryan Williams
  Revised: 5.23.2019
*/

#include "joystick.h"
#include "humidity.h"
#include "sliders.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x00;
enum States {IDLING, INIT, SWISSHOLES, JOYLRLRLR, PULLBACK, MOISTURE60, 
             MOISTURE30, FERMENTATION0, MAKEYMAKEY, PASSTHROUGH, SUCCESS};

bool debug_mode = false;
uint8_t makey_pin = 2;   //SET THIS FOR MAKEYMAKEY!
void run_mode();
void read_serial();

volatile enum States state = IDLING;

void setup() {
  Serial.begin(9600);
  joySetup(debug_mode);
  humSetup(debug_mode);
  slider_setup(debug_mode);
  ledSetup();
  pinMode(makey_pin, OUTPUT);
}

void loop() {
  run_mode();
  ledUpdate();  
  delay(1);
}

void run_mode() {
  read_serial();
  switch (state)
  {
    case IDLING:
      if (ledGetFlashDone()) {
        ledSetState(LED_IDLE);
      }
      break;
    case SWISSHOLES:       /// humidity check needs basepoint set in read_serial
      if ((humCheck)) {
        state = SUCCESS;
      }
      break;
    case JOYLRLRLR:       // Joystick check jerky
      if (joyUpdate()) {
        state = SUCCESS;
      }
      break;
    case PULLBACK:       // Joystick check up
      if (joyUpdate()) {
        state = SUCCESS;
      }
      break;
    case MOISTURE60:       // Slider check
      if (slider_check()) {
        state = SUCCESS;
      }
      break;
    case MOISTURE30:       // Slider check
      if (slider_check()) {
        state = SUCCESS;
      }
      break;
    case FERMENTATION0:
      if (slider_check()) {
        state = SUCCESS;
      }
      break;
    case MAKEYMAKEY:
      if (digitalRead(makey_pin)) {
        state = SUCCESS;
      }
      break;
    case INIT:
      if (slider_check()) {
        state = SUCCESS;
      }
      break;
    case PASSTHROUGH:       // All fake actions are passthroughs
      state = SUCCESS;
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
        Serial.write((byte)0x00);       //I'm idling
      } else {
        if (debug_mode) Serial.println("I'm trying");
        Serial.write((byte)0x01);       //I'm trying hard
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
      ledSetState(LED_FIRE);
      humSetBasePt();
      state = SWISSHOLES;
      break;
    case 0x11:
      if (debug_mode) Serial.println("drill stuck in rind / joystick up");
      ledSetState(LED_ACTIVE);
      joySetSequence(0);
      state = PULLBACK;
      break;
    case 0x12:
      if (debug_mode) Serial.println("Hit hard cheese / Joystick JOYLRLRLR");
      ledSetState(LED_ACTIVE);
      joySetSequence(1);
      state = JOYLRLRLR;
      break;
    case 0x13:
      if (debug_mode) Serial.println("gaseous cheese / turn down engines");
      ledSetState(LED_ACTIVE);
      slider_select(0);
      slider_set_sequence(2);
      state = MOISTURE60;
      break;
    case 0x14:
      if (debug_mode) Serial.println("whey lake / both sliders to 40%");
      ledSetState(LED_ACTIVE);
      slider_select(0);
      slider_set_sequence(0);
      state = MOISTURE30;
      break;
    case 0x15:
      ledSetState(LED_ACTIVE);
      slider_select(0);
      slider_set_sequence(2);
      state = FERMENTATION0;
      break;
    case 0x16:
      ledSetState(LED_ACTIVE);
      state = MAKEYMAKEY;
      break;
    case 0x17:
      ledSetState(LED_ACTIVE);
      slider_select(1);
      slider_set_sequence(1);
      state = INIT;
      break;
    case 0x19:
      state = PASSTHROUGH;
      break;          
  }
}
