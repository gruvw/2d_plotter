#include "Arduino.h"

#define STEPS 4
#define STEP_DELAY delay(2)
#define LIMIT_X 100
#define LIMIT_Y 600
#define AREA_SIDE 3000

typedef struct {
    int STEP[STEPS];
    unsigned step;
    unsigned pos;
} Stepper;

typedef struct {
    Stepper X;
    Stepper Y;
} Axes2D;

const int limit_switch = DD7;

Axes2D stepper_setup() {
    pinMode(limit_switch, INPUT_PULLUP);

    Stepper X = {DD5, DD4, DD3, DD2};
    Stepper Y = {A0, A1, A2, A3};
    return (Axes2D) {X, Y};
}

void step(Stepper * stepper, bool forward) {
    int diff = forward ? 1 : -1;
    stepper->pos += diff;

    for (int i = 0; i < STEPS; i++) {
        digitalWrite(stepper->STEP[i], (stepper->pos % STEPS - i) == 0);
    }
}

void origin_axis(Stepper * stepper, unsigned limit) {
    while (!digitalRead(limit_switch)) {
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

    while (!digitalRead(limit_switch));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->X, true);
        STEP_DELAY;
    }
    while (!digitalRead(limit_switch));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->Y, true);
        STEP_DELAY;
    }
    while (!digitalRead(limit_switch));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->X, false);
        STEP_DELAY;
    }
    while (!digitalRead(limit_switch));
    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->Y, false);
        STEP_DELAY;
    }
}
