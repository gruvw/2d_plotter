#include "start.h"

#include <stdio.h>

#include "../draw/draw.h"
#include "../draw/hilbert.h"
#include "../hardware/plotter.h"
#include "../turtle/turtle.h"

// Procedures settings
#define HILBERT_LEVEL 4
#define CIRCLE_PRECISION 15

// Procedures

// Draws a square and inscribes a circle inside without lifting the penup;
// matches the starting point of the square with the starting and ending point of the circle
void square_inscribe_circle(Plotter * plotter) {
    draw_line_to(plotter->axes, AREA_SIDE / 2, 0);
    limit_wait();

    pendown(plotter->pen);

    draw_line_to(plotter->axes, AREA_SIDE, 0);
    draw_line_to(plotter->axes, AREA_SIDE, AREA_SIDE);
    draw_line_to(plotter->axes, 0, AREA_SIDE);
    draw_line_to(plotter->axes, 0, 0);
    draw_line_to(plotter->axes, AREA_SIDE / 2, 0);

    draw_circle(plotter->axes, AREA_SIDE / 2, 0, CIRCLE_PRECISION);
}

// Draws a Hilbert spacing filling curve on the whole drawing area
void hilbert_filling(Plotter * plotter) {
    limit_wait();

    pendown(plotter->pen);

    draw_hilbert(plotter->axes, AREA_SIDE, HILBERT_LEVEL);
}

// Launches the turtle REPL
// sets the origin of the turtle drawing to the center of the drawing area
void turtle_REPL(Plotter * plotter) {
    Turtle turtle = {plotter, false, 0};  // penup by default

    // Move to center of drawing area
    draw_line_to(plotter->axes, AREA_SIDE / 2, AREA_SIDE / 2);

    turtle_main(&turtle);
}

void start() {
    // Setup
    Servo servo = servo_setup();
    Axes2D axes = stepper_setup();
    Plotter plotter = {&axes, &servo};

    // Moves to origin
    penup(plotter.pen);
    origin(plotter.axes);

    // square_inscribe_circle(&plotter);
    // hilbert_filling(&plotter);
    turtle_REPL(&plotter);

    // Lifting pen rsidue test
    // const int mult = 10;
    // const int dist = AREA_SIDE / mult;
    // for (int i = 0; i < mult; ++i) {
    //     pen_up(&servo);
    //     draw_line_to(&axes, 0, dist * i);
    //     pen_down(&servo);
    //     for (int j = 0; j < mult; ++j) {
    //         draw_line_to(&axes, dist * j, axes.Y.pos);
    //         if (j % 2 == 0) {
    //             pen_up(&servo);
    //         } else {
    //             pen_down(&servo);
    //         }
    //     }
    // }
    // pen_up(&servo);
}
