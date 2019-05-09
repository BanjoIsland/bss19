#ifndef joystick_h
#define joystick_h

#define SW_PIN A2  // digital pin connected to switch output
#define X_PIN 0 // analog pin connected to X output  
#define Y_PIN 1 // analog pin connected to Y output

void joySetup();

void joyRead();

bool joyCheck();

void joyCompare();

bool joySequence();

#endif
