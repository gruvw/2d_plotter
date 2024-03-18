#pragma once

#include "servo.h"
#include "stepper.h"

typedef struct {
    Axes2D * axes;
    Servo * pen;
} Plotter;
