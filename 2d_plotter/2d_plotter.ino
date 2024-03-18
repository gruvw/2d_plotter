extern "C" {
    #include "src/main/start.h"
}

// Standard output setup
FILE f_out;
int sput(char c, __attribute__((unused)) FILE * f) {
    return !Serial.write(c);
}

void setup() {
    // Serial input / output setup
    // cf https://www.nongnu.org/avr-libc/user-manual/group__avr__stdio.html#gaf41f158c022cbb6203ccd87d27301226
    Serial.begin(9600);
    fdev_setup_stream(&f_out, sput, nullptr, _FDEV_SETUP_WRITE);
    stdout = &f_out;

    // Program entry point
    start();
}

void loop() {
}
