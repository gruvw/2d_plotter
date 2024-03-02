#include "function.h"

#include "stepper_lib.h"

void apply_function(Axes2D * axes, Function2D function) {
    unsigned t = 0;
    for (unsigned t = 0;; ++t) {
        Step2D res = function(t, axes->X.pos, axes->Y.pos);
        if (res.finished) {
            return;
        }
        apply(axes, res.x, res.y);
    }
}
