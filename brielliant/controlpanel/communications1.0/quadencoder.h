#ifndef quadencoder_h
#define quadencoder_h

#define CLK A0 
#define DT A1
//#define QBT A2 

void encoder_setup(bool mode);

void set_encoder_count(uint16_t val);

bool encoder_check();

void read_encoder();

bool encRotateRightCheck();
bool encRotateLeftCheck();
void encReadCount();

#endif
