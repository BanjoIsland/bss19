//
// Created by Lasciel on 8/13/2016.
//
#include <FastLED.h>

#ifndef LED_H
#define LED_H

#define LED_PIN     A2
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    13 
#define LED_BASE   0

#define BRIGHTNESS  100
#define LED_FLASH_RATE 500UL

#define LED_OFF 0
#define LED_IDLE 1
#define LED_VICTORY 2
#define LED_FLASH_GREEN 3
#define LED_FLASH_RED 4
#define LED_ACTIVE 5
#define LED_FIRE 6

void ledUpdate();

void ledSetup();

void resetLed();

void ledYellow();

void ledPurple();

void ledSetState(int state);

bool ledGetFlashDone();

void setFlashCount(int count);

#endif //PUZZLETEMPLATE_LED_H
