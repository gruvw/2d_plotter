extern "C" {
    #include "stepper_lib.h"
}

void setup() {
    Axes2D axes = stepper_setup();
    origin(&axes);

    for (int i = 0; i <= 6000; ++i) {
        apply(&axes, FORWARD, FORWARD);
    }
    for (int i = 0; i <= 6000; ++i) {
        apply(&axes, BACKWARD, BACKWARD);
    }
}

void loop() {
}
