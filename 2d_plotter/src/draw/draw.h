#pragma once

#include "../hardware/stepper.h"

#define TO_RAD(x) ((x) * M_PI / 180)

void draw_line_to(Axes2D * axes, int dest_x, int dest_y);
void draw_circle(Axes2D * axes, int radius, int start_angle, int precision);
