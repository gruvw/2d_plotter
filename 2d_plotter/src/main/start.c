#include "start.h"

#include "../draw/draw.h"
#include "../draw/hilbert.h"

#define HILBERT_LEVEL 4
#define CIRCLE_PRECISION 15

// Programming assignment 1
void square_inscribe_circle(Axes2D * const axes) {
    draw_line_to(axes, AREA_SIDE / 2, 0);
    limit_wait();

    draw_line_to(axes, AREA_SIDE, 0);
    draw_line_to(axes, AREA_SIDE, AREA_SIDE);
    draw_line_to(axes, 0, AREA_SIDE);
    draw_line_to(axes, 0, 0);
    draw_line_to(axes, AREA_SIDE / 2, 0);

    draw_circle(axes, AREA_SIDE / 2, CIRCLE_PRECISION);
}

// Programming assignment 2
void hilbert_filling(Axes2D * const axes) {
    limit_wait();
    draw_hilbert(axes, AREA_SIDE, HILBERT_LEVEL);
}

void start() {
    Axes2D axes = stepper_setup();
    origin(&axes);

    square_inscribe_circle(&axes);
    // hilbert_filling(&axes);
}
