#define Sprintln(a) (Serial.println(F(a)))
//#define Sprintln(a)
#include <Arduino.h>
#include <Sleep_n0m1.h>
#include "lcd.h"
#include "config.h"
#include "messages.h"
#include "led.h"
#include "watchdog.h"
#include "puzzle_keypad.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
#include <Keypad.h>

#if PUZZLE_NUM == 0
#include "messages_puzzle0.h"
#endif
#if PUZZLE_NUM == 1
#include "messages_puzzle1.h"
#endif
#if PUZZLE_NUM == 2
#include "messages_puzzle2.h"
#endif
#if PUZZLE_NUM == 3
#include "messages_puzzle3.h"
#endif
#if PUZZLE_NUM == 4
#include "messages_puzzle4.h"
#endif
#if PUZZLE_NUM == 5
#include "messages_puzzle5.h"
#endif
#if PUZZLE_NUM == 6
#include "messages_puzzle6.h"
#endif
#if PUZZLE_NUM == 7
#include "messages_puzzle7.h"
#endif

Sleep sleep;
unsigned long sleepTime = 1000; //how long you want the arduino to sleep
#define RST_PIN         10

#define SLEEP_STATE 1
#define DISPLAY_IDLE 2
#define WAIT_FOR_GAME_DONE 3
#define DISPLAY_CHOICE_PROMPT 4
#define WAIT_MORAL_CHOICE 5
#define WAIT_FOR_GAME_DONE_TAG 6
#define WAIT_IDLE_LDE 7
#define WAIT_IDLE_OFF 8
#define WAIT_ADDITIONAL_INFO 9 
#define KEYPRESS_MESSAGE 10
#define VICTORY 11
#define DEBUG_STATE 99

int checkin_fsm_state = WAIT_IDLE_OFF;

byte playerUid[7];
byte playerName[16];
byte playerData[16];
const int playerPacketSize = sizeof(playerUid) + sizeof(playerName) + sizeof(playerData);
byte playerPacket[playerPacketSize];

int choiceEntered = 0;
unsigned long idle_demo_timer = 0;
unsigned long victory_timer = 0;
int sleepCycles = 0;

bool disableCheckOut = false;

void setup() {
     delay(5000); //to reprogram arduino before sleep cycles start
     restart();
   
}

void restart() {
    Serial.begin(9600); // Initialize serial communications with the PC
    rfidSetup();
    lcdReset();
    ledSetup();
    lcdDisplayOn();
    setLcdMessage(amazonMsg, 2, DEFAULT_LCD_SCREEN_UPDATE);
    updateLcdMessage();
    checkin_fsm_state = WAIT_IDLE_OFF;
    StartWatchdogTimer();
    idle_demo_timer = millis();
}

void loop() {
    process_state_machine();
    ledUpdate();
    if(WatchdogTimerExpired())
    {
        restart();
        Sprintln("Timer Expired!");
    }
    delay(GAME_TICK_RATE);
}

void process_state_machine()
{
    //state machine
    switch (checkin_fsm_state) {
        case SLEEP_STATE:  //Starting state to wait for RFID tag
        {

            ++sleepCycles;
            //sleepNow();
            Sprintln("Sleeping!");
            if(sleepCycles >= 5)
            {
                ledSetState(LED_IDLE);
                checkin_fsm_state = WAIT_IDLE_LDE;
                idle_demo_timer = millis();
            }
            else
            {
                ledSetState(LED_OFF);
                checkin_fsm_state = WAIT_IDLE_OFF;
                idle_demo_timer = millis();
            }
            break;
        }
        case WAIT_IDLE_OFF:
        {
            if (millis() - idle_demo_timer > IDLE_OFF_TIME) {
                checkin_fsm_state = SLEEP_STATE;
            }
            process_rfid_checkIn();
            process_keypress_checkIn();
            break;
        }
        case WAIT_IDLE_LDE: {
            if (millis() - idle_demo_timer > IDLE_DEMO_TIME) {
                sleepCycles = 0;
                checkin_fsm_state = SLEEP_STATE;
            }
            process_rfid_checkIn();
            process_keypress_checkIn();
            break;
        }
        case WAIT_FOR_GAME_DONE: {
            disableCheckOut = false;
            if (updateGameState()) {
                setFlashCount(10);
                ledSetState(LED_FLASH_GREEN);
                checkin_fsm_state = WAIT_FOR_GAME_DONE_TAG;
                Sprintln("Game Done!");
            }
            break;
        }
        case WAIT_FOR_GAME_DONE_TAG: {
            //ledSetState(LED_OFF);
            // Display name and ask to retag
            if (disableCheckOut) {
              checkin_fsm_state = SLEEP_STATE;
            } else {
              process_rfid_checkOut();
            }
            break;
        }
        case KEYPRESS_MESSAGE: {
              ledSetState(LED_OFF);
              //lcdReset();
              checkin_fsm_state = SLEEP_STATE;

            break;    
        }
        case VICTORY: {
            ResetWatchdogTimer();  // Could hang in this state, if somehow updateLcdMessage never finishes
            if(updateLcdMessage()) {
                checkin_fsm_state = SLEEP_STATE;
                restart();
            }
            break; 
        }         
        case DEBUG_STATE: {

        }
        default:
            break;
    }
}

void process_rfid_checkIn()
{
    rfidSetup();
    if (rfidDetected(playerUid, playerName, playerData, PUZZLE_TRIES_BYTE)) {
        //createPlayerPacket(playerUid, playerName, playerData, playerPacket);
        //Serial.write(playerPacket, playerPacketSize); Serial.print('\n'); 
        
        ResetWatchdogTimer();
        setupGame((char *)playerName);
        checkin_fsm_state = WAIT_FOR_GAME_DONE;  

    }
    closeRfid();
}

void process_keypress_checkIn()
{
    char key = keyCheckForKeyPress();
    if(key)
    {
      setLcdMessage(keypressCheckinPrompt, 2, DEFAULT_LCD_SCREEN_UPDATE);
      updateLcdMessage();
      checkin_fsm_state = KEYPRESS_MESSAGE;    
    }
}

void process_rfid_checkOut()
{
    rfidSetup();
    byte oldPlayerName[16];
    byte oldPlayerData[16];
    byte writeTagDone[1];
    unsigned short completedPuzzles = 0; 
    if (rfidDetected(playerUid, oldPlayerName, oldPlayerData, PUZZLE_COMPLETE_BYTE)) {

        ledSetState(LED_FLASH_GREEN);
        lcdDisplayOn();
                setLcdMessage(checkedInMsg, 4, DEFAULT_LCD_SCREEN_UPDATE);
                //updateLcdMessage();
        playAccessGrantedTone();
        checkin_fsm_state = VICTORY;
        victory_timer = millis();
    }
    closeRfid();
}

void sleepNow() {
    delay(100);
    digitalWrite(RST_PIN, LOW);
    resetLed();
    //lcdReset();
    sleep.pwrDownMode(); //set sleep mode
    sleep.sleepDelay(sleepTime); //sleep for: sleepTime
    restart();
}

void createPlayerPacket(byte playerUid[], byte playerName[], byte playerData[], byte combined[]) {
  // for some reason, using sizeof(variable) in memcpy messes up
  memcpy(combined, playerUid, 7);
  memcpy(combined+7, playerName, 16);
  memcpy(combined+23, playerData, 16);
}


