#pragma once

// Drawing area side length
#define AREA_SIDE 3000

// Stepper types

#define _STEPS 4

typedef struct {
    const int STEP_PINS[_STEPS];
    int pos;
} Stepper;

typedef struct {
    Stepper X;
    Stepper Y;
} Axes2D;

typedef enum {
    S_STAND,
    S_FORWARD,
    S_BACKWARD,
} Step;

// Returns an initialized instance of `Axes2D`
Axes2D stepper_setup();

// Moves to origin of the axes system
void origin(Axes2D * axes);

// Moves axes at the same time in the given `direction` if it is possible;
// only wait for steppers to move if one of them moved
void apply(Axes2D * axes, Step x, Step y);

// Waits till the limit switch is pressed
void limit_wait();
