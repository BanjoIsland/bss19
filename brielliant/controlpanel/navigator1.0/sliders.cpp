#include <Arduino.h>
#include "sliders.h"

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

bool debug_sliders = false;

//new slider sequences must be instantiated into the seq_struct array to be used.
int  slider_seq1[] = {1};
int slider_seq2[] = {0, 3, 0, 3, 0 ,3, 0};
int slider_seq3[] = {0};

uint8_t slider_seq_iterator = 0;
uint8_t seq_selected = 0;
uint8_t slider_selected = 0;

typedef struct seq_struct {
  int *seq;
  int num_elements;
} seq;

seq seq_list[3] =
{
  {slider_seq1, NUMELEMENTS(slider_seq1)},
  {slider_seq2, NUMELEMENTS(slider_seq2)},
  {slider_seq3, NUMELEMENTS(slider_seq3)}
};

void slider_setup(bool mode) {
  debug_sliders = mode;
  pinMode(LEFT_PIN, INPUT);
  pinMode(RIGHT_PIN, INPUT);
}
void slider_set_sequence(uint8_t seq) {
  seq_selected = seq;
}
void slider_select(uint8_t slider) {
  slider_selected = slider;
}

int left_level_check() {
	return analogRead(LEFT_PIN) / 256;
}

int right_level_check() {
	return analogRead(RIGHT_PIN) / 256;
}

bool slider_sequence_check() {
  seq this_seq = seq_list[seq_selected];
  if (slider_seq_iterator < this_seq.num_elements) {
    int curr_level;
    if (slider_selected == 0) {
      curr_level = left_level_check(); 
    } else {
      curr_level = right_level_check();
    }
    
    if (debug_sliders) {
      Serial.println("level = " + String(curr_level));
      Serial.println("iterator = " + String(slider_seq_iterator));
      Serial.println("target = " + String(*(this_seq.seq + slider_seq_iterator)));
      delay(10);
    }
    if (curr_level == *(this_seq.seq + slider_seq_iterator)) {
      slider_seq_iterator += 1;
    }
    return false;
  } else {
    slider_seq_iterator = 0;
    return true;
  }
}

bool slider_check() {
  if (slider_sequence_check()) {
    return true;
  }
  return false;
}
