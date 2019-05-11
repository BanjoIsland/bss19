#ifndef dip_h
#define dip_h

const int numDipPins = 10;
const byte dipPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 16};

void dipSetup();

void dipRead();

#endif
