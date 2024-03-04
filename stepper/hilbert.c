#include "function.h"
#include "hilbert.h"

#include <math.h>

#include "stepper_lib.h"

// Helpers to draw given a direction

// Integer -sin(pi/2 * x) length 4 bitwise mapping: 0, -1, 0, 1
#define MSIN(x) (int) (((x) & 1) - (((x) == 1) << 1))

typedef enum {
    UP,
    LEFT,
    DOWN,
    RIGHT,
    END,
} Direction;

static inline void move_dir(Axes2D * pos, Direction d, int l) {
    move_line_to(pos, pos->X.pos + MSIN(d) * l, pos->Y.pos + MSIN((d + 1) % END) * l);
}

// Hilbert curve recursive algorithm as L-system (https://en.wikipedia.org/wiki/Hilbert_curve)

void hilbert_rec(Axes2D * pos, int level, Direction direction, int length) {
    if (!--level) {
        move_dir(pos, (direction + 2) % END, length);
        move_dir(pos, RIGHT - direction, length);
        move_dir(pos, direction, length);
        return;
    }

    hilbert_rec(pos, level, (LEFT - direction + END) % END, length);
    move_dir(pos, (direction + DOWN) % END, length);
    hilbert_rec(pos, level, direction, length);
    move_dir(pos, (RIGHT - direction + END) % END, length);
    hilbert_rec(pos, level, direction, length);
    move_dir(pos, direction, length);
    hilbert_rec(pos, level, (RIGHT - direction + END) % END, length);
}

void hilbert(Axes2D * pos, int side_length, int level) {
    const int length = (int) (side_length / pow(2, level));

    hilbert_rec(pos, level, UP, length);
}
