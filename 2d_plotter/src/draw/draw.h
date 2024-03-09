#pragma once

#include <stdbool.h>

#include "../hardware/stepper.h"

typedef struct {
    bool finished;
    Step x;
    Step y;
} Step2D;

typedef Step2D (*Function2D)(long t, int x, int y, void * args);

void draw_line_to(Axes2D * axes, int dest_x, int dest_y);
void draw_circle(Axes2D * axes, int radius, int precision);
