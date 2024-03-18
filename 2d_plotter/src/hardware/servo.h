#pragma once

typedef struct {
    const int PIN;
    int pos;
} Servo;

// Returns an initialized instance of `Servo`
Servo servo_setup();

// Lifts the pen up
void penup(Servo * servo);

// Puts the pen down against the drawing area
void pendown(Servo * servo);
