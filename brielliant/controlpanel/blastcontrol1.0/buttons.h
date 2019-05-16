#ifndef buttons_h
#define buttons_h

#define BLUE_PIN 9 
#define YELLOW_PIN 7  
#define WHITE_PIN 6  
#define RED_PIN 8

#define SW1_PIN 10
#define SW2_PIN 16
#define SW3_PIN A1 


void butSetup();

bool butCheck();

bool red_check();

bool swCheck();

#endif
