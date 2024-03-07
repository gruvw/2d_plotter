#include "stepper.h"

#include <Arduino.h>

// Arduino steppers pins
#define LIMIT_PIN DD7
#define STEPPER_X_PINS { A0, A1, A2, A3 }
#define STEPPER_Y_PINS { DD5, DD4, DD3, DD2 }

// Stepper constants
#define STEP_DELAY delay(2)
#define LIMIT_X 600
#define LIMIT_Y 100

Axes2D stepper_setup() {
    pinMode(LIMIT_PIN, INPUT_PULLUP);

    Stepper X = STEPPER_X_PINS;
    Stepper Y = STEPPER_Y_PINS;
    return (Axes2D) {X, Y};
}

int mod(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

void step(Stepper * stepper, bool forward) {
    stepper->pos += forward ? 1 : -1;
    for (int p = 0; p < _STEPS; p++) {
        digitalWrite(stepper->STEP_PINS[p], (mod(stepper->pos, _STEPS) - p) == 0);
    }
}

void origin_stepper(Stepper * stepper, int limit) {
    // Walk till limit switch hit
    while (!digitalRead(LIMIT_PIN)) {
        step(stepper, false);
        STEP_DELAY;
    }

    // Walk back to relieve switch + go to axis origin
    for (int _l = 0; _l < limit; ++_l) {
        step(stepper, true);
        STEP_DELAY;
    }

    stepper->pos = 0;
}

void origin(Axes2D * axes) {
    origin_stepper(&axes->X, LIMIT_X);
    origin_stepper(&axes->Y, LIMIT_Y);
}

bool valid_direction(Stepper * stepper, Step direction) {
    return !(
        (direction == FORWARD && stepper->pos >= AREA_SIDE) ||
        (direction == BACKWARD && stepper->pos <= 0)
    );
}

bool apply_stepper(Stepper * stepper, Step direction) {
    if (direction == STAND || !valid_direction(stepper, direction)) {
        return false;
    }

    step(stepper, direction == FORWARD);
    return true;
}

void apply(Axes2D * axes, Step x, Step y) {
    // Wait only if one stepper stepped, no short-circuit evaluation
    if (apply_stepper(&axes->X, x) | apply_stepper(&axes->Y, y)) {
        STEP_DELAY;
    }
}

void limit_wait() {
    while (!digitalRead(LIMIT_PIN));
}