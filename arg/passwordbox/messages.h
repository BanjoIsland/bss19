//
// Created by Lasciel on 8/13/2016.
//

#ifndef PUZZLETEMPLATE_MESSAGES_H
#define PUZZLETEMPLATE_MESSAGES_H

#include <Arduino.h>
#include "config.h"

#define ARRAY_SIZE( array ) sizeof( array ) / sizeof( array[0] )

char verifyAmulet[2][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "Please verify",
        "your amulet now"
};

char puzzleCompleteInstructions[2][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "This puzzle is",
        "complete."
};

char endingMessage[8][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "There is a",
        "message for you",
        "You may return",
        "to Kaleidoscope",
        "830F for message",
        "or continue to",
        "solve other",
        "puzzles first"
};

#endif //PUZZLETEMPLATE_MESSAGES_H
