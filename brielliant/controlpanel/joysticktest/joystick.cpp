#include "Arduino.h"
#include "joystick.h"

enum JoyDirection {NO_DIRECTION, UP, DOWN, LEFT, RIGHT,
                   TOP_LEFT, TOP_RIGHT, BOT_LEFT, BOT_RIGHT};
enum JoyState {SEQ_1, SEQ_2};                  

struct joyStruct {
  int  xAxis;
  int  yAxis;
  int  swPin; // 1 is not pressed, 0 is pressed
};

volatile enum JoyDirection joy_direction = NO_DIRECTION;
volatile enum JoyState joy_state = SEQ_1;
joyStruct joy_read;

void joySetup() {
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);
}

void joyRead() {
  joy_read.xAxis = analogRead(X_PIN);
  joy_read.yAxis = analogRead(Y_PIN);
  joy_read.swPin = digitalRead(SW_PIN);
  /*
  Serial.print("Switch:  ");
  Serial.print(joy_pos.swPin);
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(joy_pos.xAxis);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(joy_pos.yAxis);
  Serial.print("\n\n");
  */
}

bool joyCheck() {
  joyRead();
  joyCompare(); 
  //Serial.println(joy_direction);
  if (joySequence()) {
    Serial.println("success!");
    return true; 
  }
  
}

void joyCompare() {
  if (joy_read.xAxis <= 100 && joy_read.yAxis >= 900) {
    joy_direction = TOP_LEFT;   
  } else if (joy_read.xAxis <= 100 && joy_read.yAxis >= 412 && joy_read.yAxis <= 612) {
    joy_direction = LEFT;
  } else {
    joy_direction = NO_DIRECTION;
  }
}

bool joySequence() {
  switch (joy_state) {
    case SEQ_1:
    {
      if (joy_direction == TOP_LEFT) {
        joy_state = SEQ_2;
      }
      break;
    }
    case SEQ_2:
    {
      if (joy_direction == LEFT) {
        joy_state = SEQ_1;
        return true; 
      }
      break; 
    }
    default:
      break;
  }
  return false;
}

