//
// Created by Lasciel on 8/19/2016.
//

#ifndef PUZZLETEMPLATE_MESSAGES_PUZZLE1_H_H
#define PUZZLETEMPLATE_MESSAGES_PUZZLE1_H_H

#define EXPERIENCE_ONLY_PUZZLE false

#define PASSWORD_LENGTH 3
//Note: This must be numbers!
#define PASSWORD "616"

char puzzlePrompt[4][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "Greetings",
        "",
        "What is the",
        "secret password?"
};

char checkedInMsg[4][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "PROCEED TO",
        "Briellient",
        "Mining",
        ""
};

char deniedMsg[2][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "Check in other",
        "station first"
};

char amazonMsg[2][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "Welcome to",
        "Donnie's Duck-Os"
};

#define PRESENT_CHOICE '2' 

#define ADDITIONAL_INSTRUCTIONS false

char additionalInfoPrompt[6][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "For a special",
        "puzzle,",
        "climb the",
        "tower next",
        "to the stoop",
        "in camp"
};

char keypressCheckinPrompt[2][LCD_LINE_BUFFER_LENGTH] = {
        ////////////////
        "Scan card",
        "at front of box"
};

#endif //PUZZLETEMPLATE_MESSAGES_PUZZLE1_H_H
