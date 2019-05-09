
#include <Arduino.h>
#include "humidity.h"


const uint8_t CONSOLE_ID = 0x00;
enum States {IDLING, TRYHARD_A, TRYHARD_B, TRYHARD_C, SUCCESS};
void read_serial();
volatile uint8_t state = IDLING;


void setup() {
  Serial.begin(9600);
  humSetup();
}

void loop() {
  // wait for serial command byte
  // select game state
  // check game state
  // if pass, send success byte
  read_serial();
  process_state_actions();
  //hum.checkForPass();
}

void process_state_actions() {
    switch (state)
  {
    case IDLING:
      break;
    case TRYHARD_A:       // Press B_A to succeed
      if (humCheck()) {
        state = SUCCESS;
      }
      break;
    case TRYHARD_B:       // Press B_B to succeed
      break;
    case TRYHARD_C:       // Hold B_B for >3 sec to succeed
      break;
    case SUCCESS:
      Serial.write((byte)0xFF);
      Serial.println("I won");
      state = IDLING;
      break;
  }
}

void read_serial()
{
  uint8_t incoming_byte = 0xAA; //Unused value to prevent false flags
  if (Serial.available() > 0) {
    incoming_byte = Serial.parseInt();
//    incoming_byte = Serial.read();
  }
  switch (incoming_byte)
  {
    case 0x00:        // State request; return 0x00:idling, 0x01:TRYING HARD
      if (state == IDLING) {
        Serial.println("I'm not trying");
        Serial.write((byte)0x01);       //I'm idling
      } else {
        Serial.println("I'm trying");
        Serial.write((byte)0x00);       //I'm trying hard
      }
      break;
    case 0x01:        //Identify; return console ID
      Serial.write((byte)CONSOLE_ID);
      Serial.println("You requested my ID");
      break;
    case 0x09:        //Reset
      state = IDLING;             //TODO: Think about possible consequences of this
      break;
    case 0x02:
      state = TRYHARD_A;
      Serial.println("tryhardA");
      humSetBasePt();
      break;
    case 0x03:
      state = TRYHARD_B;
      break;
    case 0x04:
      state = TRYHARD_C;
      break;
    default:
      break;
  }
}
