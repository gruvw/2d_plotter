#include "servo.h"

#include <Arduino.h>

#define SERVO_PIN 10
#define SERVO_BALANCE 20000
#define SERVO_WAIT 100

#define SERVO_PENUP 1500
#define SERVO_PENDOWN 1000

Servo servo_setup() {
    return (Servo) { SERVO_PIN, 0 };
}

void apply_servo(Servo * servo, int time) {
    delay(SERVO_WAIT);
    digitalWrite(servo->PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(servo->PIN, LOW);
    delayMicroseconds(SERVO_BALANCE - time);
    delay(SERVO_WAIT);
}

inline void pen_up(Servo * servo) {
    apply_servo(servo, SERVO_PENUP);
}

inline void pen_down(Servo * servo) {
    apply_servo(servo, SERVO_PENDOWN);
}
