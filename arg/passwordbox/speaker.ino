#include "pitches.h"

// notes in the melody1:
int melody1[] = {
        NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// notes in the melody1:
int melody2[] = {
        NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D3, NOTE_C3, NOTE_C3, NOTE_C3, NOTE_D2
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations1[] = {
        4, 8, 8, 4, 4, 4, 4, 4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations2[] = {
        4, 4, 5, 1, 4, 3, 1, 16
};

// notes in the melody1:
int accessGrantedTone[] = {
        NOTE_G5//NOTE_A3, NOTE_G4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations3[] = {
        1//8, 8
};

int accessDeniedTone[] = {
        NOTE_G4, NOTE_A3
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations4[] = {
        8, 8
};


void playTone1() {
    // iterate over the notes of the melody1:
    for (int thisNote = 0; thisNote < 8; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations1[thisNote];
        tone(A2, melody1[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(A2);
    }
}

void playTone2() {
    // iterate over the notes of the melody1:
    for (int thisNote = 0; thisNote < 8; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations2[thisNote];
        tone(A2, melody1[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(A2);
    }
}

void playAccessGrantedTone() {
    // iterate over the notes of the melody1:
    for (int thisNote = 0; thisNote < sizeof(accessGrantedTone); thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations3[thisNote];
        tone(A2, accessGrantedTone[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(A2);
    }
}

void playAccessDeniedTone() {
    // iterate over the notes of the melody1:
    for (int thisNote = 0; thisNote < sizeof(accessDeniedTone); thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations4[thisNote];
        tone(A2, accessDeniedTone[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(A2);
    }
}
