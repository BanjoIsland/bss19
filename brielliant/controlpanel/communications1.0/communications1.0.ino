/*
  BSS19 Gametech, communications officer controller v1.0
  Author: Ryan Williams
  Revised: 5.13.2019
*/

#include "humidity.h"
#include "dip.h"
#include "quadencoder.h"
#include "ultrasonic.h"
#include "led.h"

const uint8_t CONSOLE_ID = 0x02;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, TRYHARD_D, SUCCESS};

bool debug_mode = true;

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
  delay(10);
}

void run_mode() {
  read_serial();
  switch (state)
  {
    case IDLING:
      break;
    case TRYHARD_A:       // humidity check needs basepoint set in read_serial 
      if (humCheck()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_B:       // Encoder state, increment set in read_serial
      if (encoder_check()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_C:       // ultrasonic state
      if (ultraCheck()) {
        state = SUCCESS;  
      }
      break;
    case TRYHARD_D:       
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
      if (debug_mode) Serial.println("drill stuck in ring / knob 5 clicks right (1/4 turn)");
      set_encoder_count(5);
      state = TRYHARD_B;
      break;
    case 0x13:
      if (debug_mode) Serial.println("drill is leaking / cover ultrasonic");
      state = TRYHARD_C;
      break;
  }
}
