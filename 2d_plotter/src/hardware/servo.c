#include "servo.h"

#include <Arduino.h>

#define SERVO_PIN 11
#define SERVO_BALANCE 20000 + 50000

#define SERVO_PENUP 1400
#define SERVO_PENDOWN 1900

Servo servo_setup() {
    return (Servo) {SERVO_PIN, 0};
}

void apply_servo(Servo * servo, int time) {
    digitalWrite(servo->PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(servo->PIN, LOW);
    delayMicroseconds(SERVO_BALANCE - time);
}

inline void pen_up(Servo * servo) {
    apply_servo(servo, SERVO_PENUP);
}

inline void pen_down(Servo * servo) {
    apply_servo(servo, SERVO_PENDOWN);
}
