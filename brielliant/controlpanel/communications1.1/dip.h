#ifndef dip_h
#define dip_h

#define NUM_PINS 10

const byte dip_pins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 16};

void dip_setup(bool mode);

void set_dip_target(uint16_t state);

bool check_dip();

bool check_smiley();

uint16_t read_dip();

#endif
