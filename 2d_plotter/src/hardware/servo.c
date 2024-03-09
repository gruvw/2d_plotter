#include "servo.h"

#include <Arduino.h>

#define SERVO_PIN 11
#define SERVO_BALANCE 20000

Servo servo_setup() {
    return (Servo) {SERVO_PIN, 0};
}

void pen_up(Servo * servo) {
    int pos = 2500;
    digitalWrite(servo->PIN, HIGH);
    delayMicroseconds(pos);
    digitalWrite(servo->PIN, LOW);
    delayMicroseconds(SERVO_BALANCE - pos);
}

void pen_down(Servo * servo) {
    int pos = 410;
    digitalWrite(servo->PIN, HIGH);
    delayMicroseconds(pos);
    digitalWrite(servo->PIN, LOW);
    delayMicroseconds(SERVO_BALANCE - pos);
}
