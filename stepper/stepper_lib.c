#include "Arduino.h"

#define STEPS 4
#define STEP_DELAY delayMicroseconds(2000)
#define LIMIT_DIST 200

typedef struct {
    int STEP[STEPS];
    unsigned steps;
} Stepper;

typedef struct {
    unsigned x;
    unsigned y;
} Pos2D;

typedef struct {
    Stepper X;
    Stepper Y;
    Pos2D pos;
} Axes2D;

Stepper X;
Stepper Y;
Axes2D axes;

const int limit_switch = DD7;

void stepper_setup() {
    pinMode(limit_switch, INPUT_PULLUP);

    Stepper X = {DD5, DD4, DD3, DD2};
    Stepper Y = {A0, A1, A2, A3};
    axes = (Axes2D) {X, Y};
}

void step(Stepper * stepper, bool forward) {
    stepper->steps = (stepper->steps + (forward ? 1 : -1)) % STEPS;
    for (int i = 0; i < STEPS; i++) {
        digitalWrite(stepper->STEP[i], (stepper->steps - i) == 0 ? HIGH : LOW);
    }
}

void origin_axis(Stepper * stepper) {
    while (!digitalRead(limit_switch)) {
        step(stepper, false);
        STEP_DELAY;
    }

    for (unsigned i = 0; i < LIMIT_DIST; ++i) {
        step(stepper, true);
        STEP_DELAY;
    }
}

void origin(Axes2D * axes) {
    origin_axis(&axes->X);
    origin_axis(&axes->Y);
    axes->pos = (Pos2D) {0, 0};
}
