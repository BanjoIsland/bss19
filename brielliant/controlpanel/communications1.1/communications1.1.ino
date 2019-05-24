/*
  Gametech, communications officer controller v1.1
  Author: Ryan Williams
  Revised: 5.23.2019
*/

#include "humidity.h"
#include "dip.h"
#include "quadencoder.h"
#include "ultrasonic.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x02;
enum States {IDLING, INIT, SWISSHOLES, YEASTUP, YEASTDOWN, WAFTAWAY, 
             DIPS0, DIPSMILEY, PASSTHROUGH, SUCCESS};

bool debug_mode = false;

void run_mode();
void read_serial();

volatile enum States state = IDLING;

void setup() {
  Serial.begin(9600);
  humSetup(debug_mode);
  dip_setup(debug_mode);
  ultraSetup(debug_mode);
  encoder_setup(debug_mode);
  ledSetup();
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
    case SWISSHOLES:       // humidity check needs basepoint set in read_serial 
      if (humCheck()) {
        state = SUCCESS;
      }
      break;
    case YEASTUP:       // Encoder state, increment set in read_serial
      if (encRotateRightCheck()) {
        state = SUCCESS;
      }
      break;
    case YEASTDOWN:       // ultrasonic state
      if (encRotateLeftCheck()) {
        state = SUCCESS;
      }
      break;
    case WAFTAWAY: 
      if (ultraCheck()) {
        state = SUCCESS;  
      }      
      break;
    case DIPS0: 
      if (check_dip()) {
        state = SUCCESS;  
      }      
      break;
    case DIPSMILEY: 
      if (check_smiley()) {
        state = SUCCESS;  
      }      
      break;
    case INIT:
      if (check_dip()) {
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
      if (debug_mode) Serial.println("drill stuck in rind / knob full turn right");
      set_encoder_count(20);
      ledSetState(LED_ACTIVE);
      state = YEASTUP;
      break;
    case 0x12:
      if (debug_mode) Serial.println("hard cheese / rotate knob left");
      set_dip_target(2);
      ledSetState(LED_ACTIVE);
      state = YEASTDOWN;
      break;      
    case 0x13:
      if (debug_mode) Serial.println("gaseous cheese / cover ultrasonic");
      ledSetState(LED_ACTIVE);
      state = WAFTAWAY;
      break;
    case 0x14:
      if (debug_mode) Serial.println("whey lake / dip to 0");
      ledSetState(LED_ACTIVE);
      set_dip_target(0);
      state = DIPS0;
      break;
    case 0x15:
      ledSetState(LED_ACTIVE);
      set_dip_target(3);
      state = DIPSMILEY;
    case 0x16:
      state = PASSTHROUGH;
      break;
    case 0x17:
      ledSetState(LED_ACTIVE);
      set_dip_target(4);
      state = INIT;
      break;
    case 0x19:
      state = PASSTHROUGH;
      break;  
  }
}
