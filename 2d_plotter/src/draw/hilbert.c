#include "hilbert.h"

#include <math.h>

#include "draw.h"

// Helpers to draw given a direction

// Integer -sin(pi/2 * x) length 4 bitwise mapping: 0, -1, 0, 1
#define IMSIN(x) (int) (((x) & 1) - (((x) == 1) << 1))

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    END,
} Direction;

static inline void draw_line_dir(Axes2D * const pos, const Direction d, const int l) {
    draw_line_to(pos, pos->X.pos + IMSIN(d) * l, pos->Y.pos + IMSIN((d + 1) % END) * l);
}

// Hilbert curve recursive algorithm as L-system (https://en.wikipedia.org/wiki/Hilbert_curve)
void draw_hilbert_rec(Axes2D * const pos, int level, const Direction direction, const int length) {
    if (!--level) {
        draw_line_dir(pos, (direction + 2) % END, length);
        draw_line_dir(pos, RIGHT - direction, length);
        draw_line_dir(pos, direction, length);
        return;
    }

    draw_hilbert_rec(pos, level, (LEFT - direction + END) % END, length);
    draw_line_dir(pos, (direction + DOWN) % END, length);
    draw_hilbert_rec(pos, level, direction, length);
    draw_line_dir(pos, (RIGHT - direction + END) % END, length);
    draw_hilbert_rec(pos, level, direction, length);
    draw_line_dir(pos, direction, length);
    draw_hilbert_rec(pos, level, (RIGHT - direction + END) % END, length);
}

void draw_hilbert(Axes2D * const pos, const int side_length, const int level) {
    const int length = (int) (side_length / pow(2, level));

    draw_hilbert_rec(pos, level, UP, length);
}
