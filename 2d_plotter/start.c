#include "start.h"

#include "draw.h"
#include "hilbert.h"
#include "log.h"

#define FOR(len) ((t <= len || ((t -= len) && 0)))
#define DIR(a) (a < 0 ? BACKWARD : (a == 0 ? STAND : FORWARD))

Step2D square_func(long t, int x, int y) {
    static int origin = 0;

    if (FOR(AREA_SIDE / 2)) {
        return STEP(FORWARD, STAND);
    }

    if (!origin) {
        limit_wait();
        origin = !origin;
    }

    // if (FOR(AREA_SIDE / 2)) {
    //     return STEP(FORWARD, STAND);
    // }

    // if (FOR(AREA_SIDE)) {
    //     return STEP(STAND, FORWARD);
    // }

    // if (FOR(AREA_SIDE)) {
    //     return STEP(BACKWARD, STAND);
    // }

    // if (FOR(AREA_SIDE)) {
    //     return STEP(STAND, BACKWARD);
    // }

    // if (FOR(AREA_SIDE / 2)) {
    //     return STEP(FORWARD, STAND);
    // }

    return STEP_STOP;
}

// Step2D inscribed_circle_func(long t, int x, int y) {
//     static const double ac =  (double) AREA_SIDE / 2;
//     static const double precision = 90000;
//     static const double rad = TWO_PI / precision;

//     if (FOR(precision)) {
//         double a = (double) t * rad;
//         double b = round(ac * (1 + sin(a)) - x);
//         double c = round(ac * (1 - cos(a)) - y);
//         return STEP(DIR(b), DIR(c));
//     }

//     return STEP_STOP;
// }

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


    // move_line_to(&axes, AREA_SIDE / 2, 0);
    // limit_wait();

    // move_line_to(&axes, AREA_SIDE, 0);
    // move_line_to(&axes, AREA_SIDE, AREA_SIDE);
    // move_line_to(&axes, 0, AREA_SIDE);
    // move_line_to(&axes, 0, 0);
    // move_line_to(&axes, AREA_SIDE / 2, 0);

    // move_circ(&axes, AREA_SIDE / 2);

    // move_line_to(&axes, AREA_SIDE, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, AREA_SIDE);
    // move_line_to(&axes, 0, AREA_SIDE / 2);
    // move_line_to(&axes, AREA_SIDE / 2, 0);

    limit_wait();
    draw_hilbert(&axes, AREA_SIDE, 4);
}
