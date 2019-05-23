#ifndef sliders_h
#define sliders_h

#define LEFT_PIN A0
#define RIGHT_PIN A1

void slider_setup(bool mode);

void slider_set_sequence(uint8_t seq);

void slider_select(uint8_t slider); //left=0, right=1

bool slider_check();

bool slider_sequence_check();

int left_level_check();

int right_level_check();

#endif
