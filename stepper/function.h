#pragma once

#include <Arduino.h>

#include "stepper_lib.h"

typedef struct {
    bool finished;
    Step x;
    Step y;
} Step2D;

#define STEP_STOP (Step2D) { true }
#define STEP(x, y) (Step2D) { false, x, y }

typedef Step2D (*Function2D)(long t, unsigned x, unsigned y, void * args);

void apply_function(Axes2D * axes, Function2D function, void * args);
void go_to(Axes2D * axes, unsigned x1, unsigned y1);
