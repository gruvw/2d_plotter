#include <Arduino.h>

extern "C" {
  #include "input.h"
}

#define MAX_WORD_LEN 12

#define SPACE ' '
#define NEW_LINE '\n'

static inline bool is_delimiter(const char c) {
    return (c == SPACE) || (c == NEW_LINE);
}

char * get_next_word() {
    char * const word = (char *) calloc(MAX_WORD_LEN, sizeof(*word));

    do {
        while (Serial.available() <= 0);
        *word = Serial.read();
    } while (is_delimiter(*word));

    int w = 1;
    while(!is_delimiter(word[w - 1]) && w < MAX_WORD_LEN) {
        while (Serial.available() <= 0);
        word[w++] = Serial.read();
    }
    word[w - 1] = '\0';  // removes last delimiter

    return word;
}
