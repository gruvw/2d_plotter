#include "start.h"

#include "Arduino.h"
#include "function.h"
#include "stepper_lib.h"

#define FOR(len) ((t <= len || ((t -= len) && 0)))
#define DIR(a) (a < 0 ? BACKWARD : (a == 0 ? STAND : FORWARD))

Step2D square_func(unsigned t, unsigned x, unsigned y) {
    static unsigned origin = 0;

    if (FOR(AREA_SIDE / 2)) {
        return STEP(FORWARD, STAND);
    }

    if (!origin) {
        limit_wait();
        origin = !origin;
    }

    if (FOR(AREA_SIDE / 2)) {
        return STEP(FORWARD, STAND);
    }

    if (FOR(AREA_SIDE)) {
        return STEP(STAND, FORWARD);
    }

    if (FOR(AREA_SIDE)) {
        return STEP(BACKWARD, STAND);
    }

    if (FOR(AREA_SIDE)) {
        return STEP(STAND, BACKWARD);
    }

    if (FOR(AREA_SIDE / 2)) {
        return STEP(FORWARD, STAND);
    }

    return STEP_STOP;
}

// Step2D inscribed_circle_func(unsigned t, unsigned x, unsigned y) {
//     const unsigned precision = 10000;

//     if (FOR(precision)) {
//         double a = t * TWO_PI / precision;
//         return STEP(DIR(x - AREA_SIDE / 2 * cos(a)), STAND);
//     }
//     return STEP_STOP;
// }

void start() {
    Axes2D axes = stepper_setup();
    origin(&axes);

    apply_function(&axes, &square_func);
    // apply_function(&axes, &inscribed_circle_func);
}
