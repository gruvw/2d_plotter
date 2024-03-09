#pragma once

#include "../hardware/stepper.h"

void draw_line_to(Axes2D * axes, int dest_x, int dest_y);
void draw_circle(Axes2D * axes, int radius, int precision);
