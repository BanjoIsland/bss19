/*
  Gametech, pilot demolitions officer controller
  Author: Ryan Williams
  Revised: 5.12.2019
*/

#include "humidity.h"
#include "buttons.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x01;
enum States {IDLING, SWISSHOLES, CFOUR, FONDUE, ACIDATTACK, 
             LACTATE_CURD, PASSTHROUGH, SUCCESS};

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
    case CFOUR:       // button check "B4"
      if (redCheck()) {
        state = SUCCESS;
      }
      break;
    case FONDUE:       // switch check
      if (fondue_heater()) {
        state = SUCCESS;   
      }
      break;
    case ACIDATTACK:       // switch check
      if (acid_attack()) {
        state = SUCCESS;   
      }
      break; 
    case LACTATE_CURD:       // whey lake, prime curds and lactate
      if (curd_primer() && lactation()) {
        state = SUCCESS;   
      }
      break;
    case PASSTHROUGH:       // All fake actions are passthroughs
      state = SUCCESS;
      break;         
    case SUCCESS:
      Serial.write((byte) 0xFF); // So this happens at time 0
      if (debug_mode) Serial.println("I won");
      state = IDLING;
      setFlashCount(3); // I think the LED code is blocking
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
      if (debug_mode) Serial.println("drill is overheating / humidity");
      ledSetState(LED_FIRE);
      humSetBasePt();
      state = SWISSHOLES;
      break;
    case 0x11:
      if (debug_mode) Serial.println("drill is stuck in rind / red button");
      ledSetState(LED_ACTIVE);
      state = CFOUR;
      break;
    case 0x12:
      if (debug_mode) Serial.println("hard cheese / fondue heater");
      ledSetState(LED_ACTIVE);
      state = FONDUE;
      break;
    case 0x13:
      if (debug_mode) Serial.println("gaeous cheese/ initiate acid counterattack");
      ledSetState(LED_ACTIVE);
      state = ACIDATTACK;
      break;
    case 0x14:
      if (debug_mode) Serial.println("whey lake / cheese curdler and lactate");
      ledSetState(LED_ACTIVE);
      state = LACTATE_CURD;
      break;
    case 0x19:
      state = PASSTHROUGH;
      break;        
  }
}
