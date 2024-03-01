#include "stepper_lib.c"

void setup() {
    Axes2D axes = stepper_setup();
    origin(&axes);
}

void loop() {
}
