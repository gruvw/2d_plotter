#include "Arduino.h"

#define STEPS 4
#define STEP_DELAY delayMicroseconds(2000)
#define LIMIT_X 100
#define LIMIT_Y 600
#define AREA_SIDE 3000

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

void origin_axis(Stepper * stepper, unsigned limit) {
    while (!digitalRead(limit_switch)) {
        step(stepper, false);
        STEP_DELAY;
    }

    for (unsigned i = 0; i < limit; ++i) {
        step(stepper, true);
        STEP_DELAY;
    }
}

void origin(Axes2D * axes) {
    origin_axis(&axes->X, LIMIT_X);
    origin_axis(&axes->Y, LIMIT_Y);
    axes->pos = (Pos2D) {0, 0};

    for (unsigned i = 0; i < AREA_SIDE; ++i) {
        step(&axes->Y, true);
        step(&axes->X, true);
        STEP_DELAY;
    }
    // for (unsigned i = 0; i < AREA_SIDE; ++i) {
    //     step(&axes->Y, true);
    //     STEP_DELAY;
    // }
    // for (unsigned i = 0; i < AREA_SIDE; ++i) {
    //     step(&axes->X, false);
    //     STEP_DELAY;
    // }
    // for (unsigned i = 0; i < AREA_SIDE; ++i) {
    //     step(&axes->Y, false);
    //     STEP_DELAY;
    // }
}
