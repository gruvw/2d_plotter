#include "function.h"

#include "stepper_lib.h"

#define DIR(a) ((a) < 0 ? BACKWARD : ((a) == 0 ? STAND : FORWARD))

void apply_function(Axes2D * axes, Function2D function, void * args) {
    for (long t = 0;; ++t) {
        Step2D res = function(t, axes->X.pos, axes->Y.pos, args);
        if (res.finished) {
            return;
        }
        apply(axes, res.x, res.y);
    }
}

void move_line_to(Axes2D * axes, int dest_x, int dest_y) {
    int x = axes->X.pos, y = axes->Y.pos;
    const int dx = abs(dest_x - x), dy = abs(dest_y - y);
    const int sx = (x < dest_x) ? 1 : -1, sy = (y < dest_y) ? 1 : -1;
    int err = dx - dy;

    while (x != dest_x || y != dest_y) {
        apply(axes, DIR(x - axes->X.pos), DIR(y - axes->Y.pos));
        int e2 = 2 * err;
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
