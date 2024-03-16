#include "stepper.h"

#include <Arduino.h>

// Arduino steppers pins
#define LIMIT_PIN DD7
#define STEPPER_X_PINS { A0, A1, A2, A3 }
#define STEPPER_Y_PINS { DD5, DD4, DD3, DD2 }

// Stepper constants
#define STEP_DELAY delay(6)

// Static pen holder
// #define LIMIT_X 600
// #define LIMIT_Y 100

// Dynamic pen holder
#define LIMIT_X 700
#define LIMIT_Y 500

Axes2D stepper_setup() {
    pinMode(LIMIT_PIN, INPUT_PULLUP);

    Stepper X = STEPPER_X_PINS;
    Stepper Y = STEPPER_Y_PINS;
    return (Axes2D) {X, Y};
}

static inline int mod(const int a, const int b) {
    const int r = a % b;
    return r < 0 ? r + b : r;
}

void step(Stepper * const stepper, const bool forward) {
    stepper->pos += forward ? 1 : -1;
    for (int p = 0; p < _STEPS; p++) {
        digitalWrite(stepper->STEP_PINS[p], (mod(stepper->pos, _STEPS) - p) == 0);
    }
}

void origin_stepper(Stepper * const stepper, const int limit) {
    // Walk till limit switch hit
    while (!digitalRead(LIMIT_PIN)) {
        step(stepper, false);
        STEP_DELAY;
    }

    // Walk back to relieve switch + go to axis origin
    stepper->pos = -limit;
    while (stepper->pos != 0) {
        step(stepper, true);
        STEP_DELAY;
    }
}

void origin(Axes2D * const axes) {
    origin_stepper(&axes->X, LIMIT_X);
    origin_stepper(&axes->Y, LIMIT_Y);
}

static inline bool valid_direction(Stepper * const stepper, const Step direction) {
    return !(
        (direction == S_FORWARD && stepper->pos > AREA_SIDE) ||
        (direction == S_BACKWARD && stepper->pos < 0)
    );
}

bool apply_stepper(Stepper * const stepper, const Step direction) {
    if (direction == S_STAND || !valid_direction(stepper, direction)) {
        return false;
    }

    step(stepper, direction == S_FORWARD);
    return true;
}

void apply(Axes2D * const axes, const Step x, const Step y) {
    // Wait only if one stepper stepped, no short-circuit evaluation
    if (apply_stepper(&axes->X, x) | apply_stepper(&axes->Y, y)) {
        STEP_DELAY;
    }
}

void limit_wait() {
    while (!digitalRead(LIMIT_PIN));
}
