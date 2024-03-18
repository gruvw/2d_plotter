#pragma once

#include <stdbool.h>

#include "../hardware/plotter.h"

typedef struct {
    Plotter * plotter;
    bool is_pendown;
    int angle;  // in degrees
} Turtle;

// Turtle REPL from standard input
void turtle_main(Turtle * turtle);
