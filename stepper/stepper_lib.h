#pragma once

#include "Arduino.h"

#define _STEPS 4
#define AREA_SIDE 3000

typedef struct {
    int STEP[_STEPS];
    unsigned step;
    unsigned pos;
} Stepper;

typedef struct {
    Stepper X;
    Stepper Y;
} Axes2D;

Axes2D stepper_setup();
void origin(Axes2D * axes);
