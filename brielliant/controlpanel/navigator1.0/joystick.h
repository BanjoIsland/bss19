#ifndef joystick_h
#define joystick_h

#define UP_PIN 9  // digital pin connected to switch output
#define DOWN_PIN 7  // digital pin connected to switch output
#define LEFT_PIN 8  // digital pin connected to switch output
#define RIGHT_PIN 6  // digital pin connected to switch output

#define CENTER 0
#define UP 1
#define UP_RIGHT 2
#define RIGHT 3
#define DOWN_RIGHT 4
#define DOWN 5
#define DOWN_LEFT 6
#define LEFT 7
#define UP_LEFT 8


void joySetup(bool mode);

void joyRead();

void joySetSequence(uint16_t sequence);

bool joyUpdate();

bool joyCheckSequence();

#endif
