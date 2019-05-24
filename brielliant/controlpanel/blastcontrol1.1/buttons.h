#ifndef buttons_h
#define buttons_h

#define BLUE_PIN 9 
#define YELLOW_PIN 7  
#define GREEN_PIN 8  
#define RED_PIN 6

#define SW1_PIN 3
#define SW2_PIN 10
#define SW3_PIN 14 


void butSetup();

bool butAllCheck();

bool redCheck();
bool greenCheck();
bool blueCheck();
bool yellowCheck();

bool sw1Check();
bool sw2Check();
bool sw3Check();
bool swAllHighCheck();
bool swAllLowCheck();

int curd_primer();
int lactation();
int fondue_heater();
int acid_attack();
int enzymatic();

#endif
