#include <Arduino.h>

extern "C" {
  #include "input.h"
}

#define MAX_WORD_LEN 10
#define SPACE ' '
#define NEW_LINE '\n'

static inline bool is_delimiter(char c) {
    return (c == SPACE) || (c == NEW_LINE);
}

char * get_next_word() {
    char * const word = (char *) calloc(MAX_WORD_LEN, sizeof(*word));

    do {
        while (Serial.available() <= 0);
        *word = Serial.read();
    } while (is_delimiter(*word));

    for (int i = 1; i < MAX_WORD_LEN && !is_delimiter(word[i]); ++i) {
        while (Serial.available() <= 0);
        word[i] = Serial.read();
    }

    return word;
}
