#include <Arduino.h>
#include <stdbool.h>

extern "C" {
    #include "buzzer.h"
}

// Buzzer Arduino Settings
#define BUZZER_PIN 8
#define SOUND_ON true
#define BUZZER_END_WAIT 300

// Notes frequencies
#define NOTE_C 523
#define NOTE_CS 554
#define NOTE_D 587
#define NOTE_DS 622
#define NOTE_E 659
#define NOTE_F 698
#define NOTE_FS 740
#define NOTE_G 784
#define NOTE_GS 831
#define NOTE_A 880
#define NOTE_AS 932
#define NOTE_B 988

// Plays the given frequency `freq` for `duration` milli-sec on buzzer if `SOUND_ON` is true, nothing otherwise
void buzz(int freq, int duration) {
    if (!SOUND_ON) {
        return;
    }

    pinMode(BUZZER_PIN, OUTPUT);
    tone(BUZZER_PIN, freq);
    delay(duration);
    noTone(BUZZER_PIN);
}

void play_start_sound() {
    buzz(NOTE_C, 100);
    delay(20);
    buzz(NOTE_E, 100);
    delay(20);
    buzz(NOTE_G, 100);

    delay(800);

    buzz(NOTE_G, 200);
    delay(50);
    buzz(NOTE_G, 200);

    delay(BUZZER_END_WAIT);
}

void play_ready_sound() {
    buzz(NOTE_C, 150);
    delay(100);

    buzz(NOTE_G, 110);
    delay(30);
    buzz(NOTE_G, 110);

    delay(BUZZER_END_WAIT);
}

void play_done_sound() {
    buzz(NOTE_G, 150);
    delay(50);
    buzz(NOTE_C, 150);
    delay(50);
    buzz(NOTE_G, 150);

    delay(BUZZER_END_WAIT);
}
