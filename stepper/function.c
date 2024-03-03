#include "function.h"
#include <limits.h>

#include "stepper_lib.h"
#include "log.h"

#define DIR(a) ((a) < 0 ? BACKWARD : ((a) == 0 ? STAND : FORWARD))
#define CIR(x, y) ((x) * (x) + (y) * (y))

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

// void draw_circ(Axes2D * axes, int radius, int rev, int xm, int ym, int d_rev) {
//     int t1 = radius / 16;
//     int x = radius;
//     int y = 0;

//     while (x >= y) {
//         int x0 = rev ? y : x;
//         int y0 = rev ? x : y;
//         x0 = xm ? -x0 : x0;
//         y0 = ym ? -y0 : y0;
//         a[y][0] = x0 + radius;
//         a[y][1] = y0 + radius;

//         y = y + 1;
//         t1 = t1 + y;
//         int t2 = t1 - x;

//         if (t2 >= 0) {
//             t1 = t2;
//             x = x - 1;
//         }
//     }

//     for (int i = 0; i < y; ++i) {
//         int idx = (!d_rev) ? i : (y - 1 - i);
//         log_double(a[ixd][])
//         apply(axes, DIR(a[idx][0] - axes->X.pos), DIR(a[idx][1] - axes->Y.pos));
//     }
// }

// void move_circ(Axes2D * axes, int radius) {
//     draw_circ(axes, radius, 1, 0, 1, 0);
//     draw_circ(axes, radius, 0, 0, 1, 1);

//     draw_circ(axes, radius, 0, 0, 0, 0);
//     draw_circ(axes, radius, 1, 0, 0, 1);

//     draw_circ(axes, radius, 1, 1, 0, 0);
//     draw_circ(axes, radius, 0, 1, 0, 1);

//     draw_circ(axes, radius, 0, 1, 1, 0);
//     draw_circ(axes, radius, 1, 1, 1, 1);
// }

void draw_circ(Axes2D * axes, long radius, int i, int j) {
    long x = axes->X.pos, y = axes->Y.pos;
    long a[3][2] = {
        {x + i, y},
        {x, y + j},
        {x + i, y + j},
    };
    long best_i = 0;
    long best_err = LONG_MAX;
    for (int i = 0; i < 3; ++i) {
        long pos = CIR(a[i][0] - radius, a[i][1] - radius);
        long err = abs(pos - radius * radius);
        if (err < best_err) {
            best_err = err;
            best_i = i;
        }
    }
    apply(axes, DIR(a[best_i][0] - axes->X.pos), DIR(a[best_i][1] - axes->Y.pos));
}

void move_circ(Axes2D * axes, long radius) {
    while (!(axes->X.pos == 2 * radius && axes->Y.pos == radius)) {
        draw_circ(axes, radius, 1, 1);
    }
    while (!(axes->X.pos == radius && axes->Y.pos == 2 * radius)) {
        draw_circ(axes, radius, -1, 1);
    }
    while (axes->X.pos != 0 || axes->Y.pos == radius) {
        draw_circ(axes, radius, -1, -1);
    }
    while (axes->X.pos != radius || axes->Y.pos != 0) {
        draw_circ(axes, radius, 1, -1);
    }
}
