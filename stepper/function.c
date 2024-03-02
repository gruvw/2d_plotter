#include "function.h"

#include "stepper_lib.h"
#include "log.h"

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

void plot_line_low(Axes2D * axes, unsigned x1, unsigned y1) {
    int x = (int) axes->X.pos, y = (int) axes->Y.pos;
    const int dx = (int) x1 - x;
    int dy = (int) y1 - y;
    int yi = 1;
    if (dy < 0) {
        yi = -1;
        dy = -dy;
    }
    int D = 2 * dy - dx;

    for (; x < x1; ++x) {
        log_double(y);
        apply(axes, DIR(x - axes->X.pos), DIR(y - axes->Y.pos));
        if (D > 0) {
            y = y + yi;
            D = D + (2 * (dy - dx));
        } else {
            D = D + 2*dy;
        }
    }
}

void plot_line_high(Axes2D * axes, unsigned x1, unsigned y1) {
    int x = axes->X.pos, y = axes->Y.pos;
    int dx = x1 - x;
    const int dy = y1 - y;
    int xi = 1;
    if (dx < 0) {
        xi = -1;
        dx = -dx;
    }
    int D = 2 * dx - dy;

    for (; y < y1; ++y) {
        apply(axes, DIR(x - axes->X.pos), DIR(y - axes->Y.pos));
        if (D > 0) {
            x = x + xi;
            D = D + (2 * (dx - dy));
        } else {
            D = D + 2*dx;
        }
    }
}

void go_to(Axes2D * axes, unsigned x1, unsigned y1) {
    plot_line_low(axes, x1, y1);
}
