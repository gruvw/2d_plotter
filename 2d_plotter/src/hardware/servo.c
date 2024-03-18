#include "servo.h"

#include <Arduino.h>

// Servo arduino settings
#define SERVO_PIN 10
#define SERVO_BALANCE 20000
#define SERVO_WAIT 100

// Servo angles (in micro-sec) for penup / pendown
#define SERVO_PENUP 1500
#define SERVO_PENDOWN 1000

Servo servo_setup() {
    return (Servo) {SERVO_PIN, 0};
}

// Applies the given angle `time` to the `servo`
void apply_servo(Servo * servo, int time) {
    delay(SERVO_WAIT);
    digitalWrite(servo->PIN, HIGH);
    delayMicroseconds(time);
    digitalWrite(servo->PIN, LOW);
    delayMicroseconds(SERVO_BALANCE - time);
    delay(SERVO_WAIT);
}

inline void penup(Servo * servo) {
    apply_servo(servo, SERVO_PENUP);
}

inline void pendown(Servo * servo) {
    apply_servo(servo, SERVO_PENDOWN);
}
