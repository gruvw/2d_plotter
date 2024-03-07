#include "draw.h"

#include <limits.h>
#include <math.h>
#include <stdlib.h>

#include "stepper.h"

#define DIR(a) ((a) < 0 ? BACKWARD : ((a) == 0 ? STAND : FORWARD))
#define CIR(d, r) (round(d * radius))

// Modified Bresenham's line algorithm (https://en.wikipedia.org/wiki/Bresenhams_line_algorithm)
void draw_line_to(Axes2D * axes, int dest_x, int dest_y) {
    int x = axes->X.pos, y = axes->Y.pos;
    const int dx = abs(dest_x - x), dy = abs(dest_y - y);
    const int sx = (x < dest_x) ? 1 : -1, sy = (y < dest_y) ? 1 : -1;
    int err = dx - dy, e2 = 0;

    while (axes->X.pos != dest_x || axes->Y.pos != dest_y) {
        apply(axes, DIR(x - axes->X.pos), DIR(y - axes->Y.pos));
        e2 = err << 1;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
    }
}

void draw_circle(Axes2D * axes, int radius, int precision) {
    const int x = axes->X.pos, y = axes->Y.pos;
    const double step = M_PI_2 / precision;

    for (int t = 1; t <= 4 * precision; ++t) {
        double dest_x = sin(t * step), dest_y = -cos(t * step) + 1;
        draw_line_to(axes, CIR(dest_x, radius) + x, CIR(dest_y, radius) + y);
    }

    draw_line_to(axes, x, y);
}
