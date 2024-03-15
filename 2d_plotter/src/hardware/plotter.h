#pragma once

#include "stepper.h"
#include "servo.h"

typedef struct {
    Axes2D * axes;
    Servo * pen;
} Plotter;
