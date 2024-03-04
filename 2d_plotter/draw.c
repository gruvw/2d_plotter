#include "draw.h"

#include <limits.h>

#define DIR(a) ((a) < 0 ? BACKWARD : ((a) == 0 ? STAND : FORWARD))
#define CIR(x, y) apply(axes, DIR((x) - axes->X.pos), DIR((y) - axes->Y.pos));

void apply_function(Axes2D * axes, Function2D function, void * args) {
    for (long t = 0;; ++t) {
        Step2D res = function(t, axes->X.pos, axes->Y.pos, args);
        if (res.finished) {
            return;
        }
        apply(axes, res.x, res.y);
    }
}

void draw_line_to(Axes2D * axes, int dest_x, int dest_y) {
    int x = axes->X.pos, y = axes->Y.pos;
    const int dx = abs(dest_x - x), dy = abs(dest_y - y);
    const int sx = (x < dest_x) ? 1 : -1, sy = (y < dest_y) ? 1 : -1;
    int err = dx - dy;

    while (axes->X.pos != dest_x || axes->Y.pos != dest_y) {
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

void draw_octant_circ(Axes2D * axes, int radius, int rev, int xm, int ym, int d_rev) {
    if (!d_rev) {
        int x = radius;
        int y = 0;
        int decision = 1 - radius;

        while (y <= x) {
            int x0 = rev ? y : x;
            int y0 = rev ? x : y;
            x0 = xm ? -x0 : x0;
            y0 = ym ? -y0 : y0;
            CIR(x0 + radius, y0 + radius);

            y++;
            if (decision <= 0) {
                decision += 2 * y + 1;
            } else {
                x--;
                decision += 2 * (y - x) + 1;
            }
        }
        return;
    }

    int x = radius;
    int y = 0;
    int decision = 1 - radius;
    int tot = 0;
    while (y <= x) {
        tot++;
        y++;
        if (decision <= 0) {
            decision += 2 * y + 1;
        } else {
            x--;
            decision += 2 * (y - x) + 1;
        }
    }


    for (int i = tot - 1; i >= 0; --i) {
        int x = radius;
        int y = 0;
        int decision = 1 - radius;
        int j = 0;

        while (y <= x) {
            if (j == i){
                int x0 = rev ? y : x;
                int y0 = rev ? x : y;
                x0 = xm ? -x0 : x0;
                y0 = ym ? -y0 : y0;
                CIR(x0 + radius, y0 + radius);
            }
            j++;
            y++;
            if (decision <= 0) {
                decision += 2 * y + 1;
            } else {
                x--;
                decision += 2 * (y - x) + 1;
            }
        }
    }
}

void draw_circ(Axes2D * axes, int radius) {
    draw_octant_circ(axes, radius, 1, 0, 1, 0);
    draw_octant_circ(axes, radius, 0, 0, 1, 1);

    draw_octant_circ(axes, radius, 0, 0, 0, 0);
    draw_octant_circ(axes, radius, 1, 0, 0, 1);

    draw_octant_circ(axes, radius, 1, 1, 0, 0);
    draw_octant_circ(axes, radius, 0, 1, 0, 1);

    draw_octant_circ(axes, radius, 0, 1, 1, 0);
    draw_octant_circ(axes, radius, 1, 1, 1, 1);
}
