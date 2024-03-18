#pragma once

#include "../hardware/stepper.h"

// Draws a Hilbert space filling curve of given `level` from current position of `side_length` side
void draw_hilbert(Axes2D * pos, int side_length, int level);
