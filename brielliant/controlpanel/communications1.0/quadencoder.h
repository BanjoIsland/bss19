#ifndef quadencoder_h
#define quadencoder_h

#define CLK A7 
#define DT A6
#define QBT A0 

void encoder_setup(bool mode);
int read_encoder();

#endif
