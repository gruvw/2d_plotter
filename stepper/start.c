#include "start.h"

#include <Arduino.h>

#include "function.h"
#include "log.h"
#include "stepper_lib.h"

#define FOR(len) ((t <= len || ((t -= len) && 0)))
#define DIR(a) (a < 0 ? BACKWARD : (a == 0 ? STAND : FORWARD))
#define CIR(x, y) ((x - ac) * (x - ac) + (y - ac) * (y - ac))

Step2D square_func(long t, unsigned x, unsigned y) {
    static unsigned origin = 0;

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

Step2D inscribed_circle_func(long t, unsigned x, unsigned y) {
    static const double precision = 100000;
    static const double rad = TWO_PI / precision;

    if (FOR(precision)) {
        double a = (double) t * rad;
        double b = round((double) AREA_SIDE / 2 * (1 + sin(a)) - x);
        if (b != 0) {
            return STEP(DIR(b), STAND);
        }
        double c = round((double) AREA_SIDE / 2 * (1 - cos(a)) - y);
        return STEP(STAND, DIR(c));
    }

    return STEP_STOP;
}

void start() {
    Axes2D axes = stepper_setup();
    origin(&axes);

    apply_function(&axes, &square_func);
    apply_function(&axes, &inscribed_circle_func);
}
