#include "Arduino.h"
#include "stepper_lib.h"

#define LIMIT_PIN DD7
#define STEPPER_X_PINS {DD5, DD4, DD3, DD2}
#define STEPPER_Y_PINS {A0, A1, A2, A3}

#define STEP_DELAY delay(2)
#define LIMIT_X 100
#define LIMIT_Y 600

Axes2D stepper_setup() {
    pinMode(LIMIT_PIN, INPUT_PULLUP);

    Stepper X = STEPPER_X_PINS;
    Stepper Y = STEPPER_Y_PINS;
    return (Axes2D) {X, Y};
}

void step(Stepper * stepper, bool forward) {
    int diff = forward ? 1 : -1;
    stepper->pos += diff;

    for (int i = 0; i < _STEPS; i++) {
        digitalWrite(stepper->STEP[i], (stepper->pos % _STEPS - i) == 0);
    }
}

void origin_axis(Stepper * stepper, unsigned limit) {
    while (!digitalRead(LIMIT_PIN)) {
        step(stepper, false);
        STEP_DELAY;
    }

    for (unsigned i = 0; i < limit; ++i) {
        step(stepper, true);
        STEP_DELAY;
    }

    stepper->pos = 0;
}

void origin(Axes2D * axes) {
    origin_axis(&axes->X, LIMIT_X);
    origin_axis(&axes->Y, LIMIT_Y);

    while (!digitalRead(LIMIT_PIN));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->X, true);
        STEP_DELAY;
    }
    while (!digitalRead(LIMIT_PIN));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->Y, true);
        STEP_DELAY;
    }
    while (!digitalRead(LIMIT_PIN));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->X, false);
        STEP_DELAY;
    }
    while (!digitalRead(LIMIT_PIN));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->Y, false);
        STEP_DELAY;
    }
}
