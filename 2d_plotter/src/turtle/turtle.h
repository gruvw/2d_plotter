#pragma once

#include <stdbool.h>

#include "../hardware/plotter.h"

typedef struct {
    Plotter * plotter;
    bool is_pendown;
    int angle;  // in degrees
} Turtle;

void turtle_main(Turtle * turtle);
