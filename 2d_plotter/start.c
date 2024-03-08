#include "start.h"

#include "draw.h"
#include "hilbert.h"
#include "log.h"

#define HILBERT_LEVEL 4
#define CIRCLE_PRECISION 15

void start() {
    Axes2D axes = stepper_setup();
    origin(&axes);

    // apply_function(&axes, &square_func);
    // apply_function(&axes, &inscribed_circle_func);

    // move_line_to(&axes, AREA_SIDE, AREA_SIDE / 4);
    // move_line_to(&axes, 0, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE, AREA_SIDE / 4);
    // move_line_to(&axes, 0, 0);

    // move_line_to(&axes, AREA_SIDE / 4, AREA_SIDE);
    // move_line_to(&axes, AREA_SIDE / 2, 0);
    // move_line_to(&axes, AREA_SIDE / 4, AREA_SIDE);
    // move_line_to(&axes, 0, 0);

    // move_line_to(&axes, AREA_SIDE, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, AREA_SIDE);
    // move_line_to(&axes, 0, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, 0);

    // move_line_to(&axes, AREA_SIDE, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, AREA_SIDE);
    // move_line_to(&axes, 0, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, 0);


    // Inscribed circle in square

    // draw_line_to(&axes, AREA_SIDE / 2, 0);
    // limit_wait();

    // draw_line_to(&axes, AREA_SIDE, 0);
    // draw_line_to(&axes, AREA_SIDE, AREA_SIDE);
    // draw_line_to(&axes, 0, AREA_SIDE);
    // draw_line_to(&axes, 0, 0);
    // draw_line_to(&axes, AREA_SIDE / 2, 0);

    // draw_circle(&axes, AREA_SIDE / 2, CIRCLE_PRECISION);


    // Hilbert

    limit_wait();
    draw_hilbert(&axes, AREA_SIDE, HILBERT_LEVEL);
}
