#include "Arduino.h"
#include "joystick.h"

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

// To add more sequences, add NUMSEQUENCES, create joySeqx array below, 
// and add to joySeqMatrix
#define NUMSEQUENCES 3
int joySeq1[] = {UP};
int joySeq2[] = {LEFT, CENTER, RIGHT, CENTER, LEFT, CENTER, RIGHT};
int joySeq3[] = {UP, CENTER, UP, CENTER, DOWN, CENTER, DOWN, CENTER,
                 LEFT, CENTER, RIGHT, CENTER, LEFT, CENTER, RIGHT};

uint16_t joySeqSelected = 0;
int joySeqIterator = 0;

int joyCurrentPos = CENTER;

struct joySeqStruct
{
  int *joySeq;
  uint16_t numElements;
};

joySeqStruct joySeqMatrix[] =
{
  {joySeq1, NUMELEMENTS(joySeq1)},
  {joySeq2, NUMELEMENTS(joySeq2)},
  {joySeq3, NUMELEMENTS(joySeq3)},
};


void joySetup() {
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(RIGHT_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);
  pinMode(LEFT_PIN, INPUT_PULLUP);
}

void joySetSequence(uint16_t sequence) {
  joySeqSelected = sequence;
}

void joyRead() {
  bool up,down,left,right = false;
  if (digitalRead(UP_PIN)==LOW) {up = true;} else {up = false;}
  if (digitalRead(DOWN_PIN)==LOW) {down = true;} else {down = false;}
  if (digitalRead(LEFT_PIN)==LOW) {left = true;} else {left = false;}
  if (digitalRead(RIGHT_PIN)==LOW) {right = true;} else {right = false;}

  joyCurrentPos = CENTER;
  if (up) joyCurrentPos = UP;
  if (down) joyCurrentPos = DOWN;
  if (left) joyCurrentPos = LEFT;
  if (right) joyCurrentPos = RIGHT;
  if (up && right) joyCurrentPos = UP_RIGHT;
  if (down && right) joyCurrentPos = DOWN_RIGHT;
  if (down && left) joyCurrentPos = DOWN_LEFT;
  if (up && left) joyCurrentPos = UP_LEFT;

  /*
  Serial.println(joyCurrentPos);
  Serial.print("UP=");
  Serial.print(digitalRead(UP_PIN));
  Serial.print("  DOWN=");
  Serial.print(digitalRead(DOWN_PIN));
  Serial.print("  LEFT=");
  Serial.print(digitalRead(LEFT_PIN));
  Serial.print("  RIGHT=");
  Serial.println(digitalRead(RIGHT_PIN));
  Serial.print("\n\n");
  */
}

bool joyUpdate() {
  joyRead();
  if (joySeqSelected < NUMSEQUENCES) {
    if (joyCheckSequence()) {
      //Serial.println("success!");
      return true; 
    } else {
      return false;
    }
  }
}

void joyCompare() {

}

bool joyCheckSequence() {
  
  if (joyCurrentPos == joySeqMatrix[joySeqSelected].joySeq[joySeqIterator]) {
    //Serial.println(joySeqMatrix[joySeqSelected].joySeq[joySeqIterator]);
    joySeqIterator += 1;
  }    

  if (joySeqIterator >= joySeqMatrix[joySeqSelected].numElements) {
    joySeqIterator = 0;
    return true;
  } else {
    return false;
  }
}

