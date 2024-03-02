#include "stepper_lib.h"

#include "Arduino.h"

// Arduino steppers pins
#define LIMIT_PIN DD7
#define STEPPER_X_PINS { DD5, DD4, DD3, DD2 }
#define STEPPER_Y_PINS { A0, A1, A2, A3 }

// Stepper constants
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
    stepper->pos += forward ? 1 : -1;
    for (int i = 0; i < _STEPS; i++) {
        digitalWrite(stepper->STEP_PINS[i], (stepper->pos % _STEPS - i) == 0);
    }
}

void origin_stepper(Stepper * stepper, unsigned limit) {
    // Walk till limit switch hit
    while (!digitalRead(LIMIT_PIN)) {
        step(stepper, false);
        STEP_DELAY;
    }

    // Walk back to relieve switch + go to axis origin
    for (unsigned i = 0; i < limit; ++i) {
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
    // Wait if one stepper stepped, no short-circuit evaluation
    if (apply_stepper(&axes->X, x) | apply_stepper(&axes->Y, y)) {
        STEP_DELAY;
    }
}

void limit_wait() {
    while (!digitalRead(LIMIT_PIN));
}
