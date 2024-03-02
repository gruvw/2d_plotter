#pragma once

// Drawing area side length
#define AREA_SIDE 3000

// Stepper types

#define _STEPS 4

typedef struct {
    const int STEP_PINS[_STEPS];
    unsigned pos;
} Stepper;

typedef struct {
    Stepper X;
    Stepper Y;
} Axes2D;

typedef enum {
    BACKWARD,
    STAND,
    FORWARD,
} Step;

// Stepper public functions

Axes2D stepper_setup();

void origin(Axes2D * axes);

void apply(Axes2D * axes, Step x, Step y);

void limit_wait();
