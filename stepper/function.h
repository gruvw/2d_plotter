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

typedef Step2D (*Function2D)(long t, int x, int y, void * args);

void apply_function(Axes2D * axes, Function2D function, void * args);
void move_line_to(Axes2D * axes, int dest_x, int dest_y);
void move_circ(Axes2D * axes, long radius);
void hilbert(Axes2D * axes);
