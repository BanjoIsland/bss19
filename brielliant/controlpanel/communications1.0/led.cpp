//
// Created by Lasciel on 8/13/2016.
//

#include "led.h"


uint8_t gHue = 0; // rotating "base color" used by many of the patterns
CRGB leds[NUM_LEDS];

int led_state = 0;

CRGB options[3] = {CRGB::Red, CRGB::Blue, CRGB::Green};



#define COOLING  20
#define SPARKING 200

static int flashCount = 3; 
static bool flashState;
static unsigned long flashTimer;
static CRGB flashColor;
static int flashSetCount;

void Fire2012()
{
// Array of temperature readings at each simulation cell
    static byte heat[NUM_LEDS];

    // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS; i++) {
        heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
    }

    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
        heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }

    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
        int y = random8(7);
        heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
        CRGB color = HeatColor( heat[j]);
        int pixelnumber;
        pixelnumber = j;
        leds[pixelnumber] = color;
    }
}


void addGlitter( fract8 chanceOfGlitter)
{
    if( random8() < chanceOfGlitter) {
        leds[ random16(NUM_LEDS) ] += CRGB::White;
    }
}

void rainbow()
{
    // FastLED's built-in rainbow generator
    // for duckOs only light up first 11 leds in box
    fill_rainbow( leds, 11, gHue, 7);
}

void rainbowWithGlitter()
{
    // built-in FastLED rainbow, plus some random sparkly glitter
    rainbow();
    addGlitter(80);
}

void confetti()
{
    // random colored speckles that blink in and fade smoothly
    fadeToBlackBy( leds, NUM_LEDS, 10);
    int pos = random16(NUM_LEDS);
    leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
    // a colored dot sweeping back and forth, with fading trails
    fadeToBlackBy( leds, NUM_LEDS, 20);
    int pos = beatsin16(13,0,NUM_LEDS);
    leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
    // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
    uint8_t BeatsPerMinute = 62;
    CRGBPalette16 palette = PartyColors_p;
    uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
    for( int i = 0; i < NUM_LEDS; i++) { //9948
        leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
    }
}

void juggle() {
    // eight colored dots, weaving in and out of sync with each other
    fadeToBlackBy( leds, NUM_LEDS, 20);
    byte dothue = 0;
    for( int i = 0; i < 8; i++) {
        leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
        dothue += 32;
    }
}

void flash()
{
    if(ledGetFlashDone())
    {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CRGB::Black;
            FastLED.show();
        }
    }
    else {
        if (millis() - flashTimer > LED_FLASH_RATE) {
            for (int i = 0; i < NUM_LEDS; i++) {
                if (flashState) {
                    leds[i] = flashColor;
                } else {
                    leds[i] = CRGB::Black;
                }
            }
            if (!flashState) {
                ++flashCount;
            }
            flashTimer = millis();
            flashState = !flashState;
        }
    }
}

void setFlashCount(int count)
{
    flashSetCount = count;
}

bool ledGetFlashDone()
{
    return flashCount + 1 > flashSetCount;
}

static unsigned long cycleSideTimer;
static int sideIndex;


void ledSetState(int state)
{
    led_state = state;
    switch(led_state) {
        case LED_FLASH_GREEN:
        {
            flashState = true;
            flashTimer = millis();
            flashColor = CRGB::Green;
            flashCount = 0;
            break;
        }
        case LED_FLASH_RED: {
            flashState = true;
            flashTimer = millis();
            flashColor = CRGB::Red;
            flashCount = 0;
            break;
        }
        default:
            break;
    }
}

void ledUpdate()
{
    switch(led_state) {
        case LED_OFF:{
            resetLed();
            break;
        }
        case LED_IDLE: {
            //rainbow();
            ledPurple();
            break;
        }
        case LED_ACTIVE: {
            //rainbow();
            ledYellow();
            break;
        }        
        case LED_VICTORY: {
            Fire2012();
            break;
        }
        case LED_FIRE: {
            Fire2012();
            break;
        }        
        case LED_FLASH_GREEN:
        {
            flash();
            break;
        }
        case LED_FLASH_RED:
        {
            flash();
            break;
        }
        default:
            break;
    }
    FastLED.show();
    EVERY_N_MILLISECONDS( 1 ) { gHue++; };
}

void ledSetup() {
    delay(1000); // sanity delay
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( BRIGHTNESS );   
}

void resetLed() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}

void ledYellow() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Yellow;
    }
    FastLED.show();
}

void ledPurple() {
    for (int i=0; i<NUM_LEDS; i++) {
        leds[i] = CRGB::Purple;
    }
    FastLED.show();
}

