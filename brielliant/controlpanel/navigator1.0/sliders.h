#ifndef sliders_h
#define sliders_h

#define LEFT_PIN A3
#define RIGHT_PIN A2

void slider_setup(bool mode);

void slider_set_sequence(uint8_t seq);

bool slider_check();

bool slider_sequence_check();

int left_level_check();

int right_level_check();

#endif
