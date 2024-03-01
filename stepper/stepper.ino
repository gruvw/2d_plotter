extern "C" {
    #include "stepper_lib.h"
}

void setup() {
    Axes2D axes = stepper_setup();
    origin(&axes);
}

void loop() {
}
