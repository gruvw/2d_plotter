#pragma once

#include <math.h>

#include "../hardware/stepper.h"

#define TO_RAD(x) ((x) * M_PI / 180)

// Draws a line from current position to the position `dest_x`, `dest_y`;
// uses a modified Bresenham's line algorithm (https://en.wikipedia.org/wiki/Bresenhams_line_algorithm)
void draw_line_to(Axes2D * axes, int dest_x, int dest_y);

// Draws a circle of given `radius`, rotated by `angle`, from current position;
// draws the circle using `4 * precision` points
void draw_circle(Axes2D * axes, int radius, int start_angle, int precision);
