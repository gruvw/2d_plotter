#define STEPPER 4
#define STEP_DELAY 2000

const int STEP_X[STEPPER] = {DD5, DD4, DD3, DD2};
const int STEP_Y[STEPPER] = {A0, A1, A2, A3};

unsigned steps_X = 0;
unsigned steps_Y = 0;

const int limit_switch = DD7;

void setup() {
}

void step(bool forward) {
    steps_X = (steps_X + (forward ? 1 : -1)) % STEPPER;
    for (int i = 0; i < STEPPER; i++) {
        digitalWrite(STEP_X[i], (steps_X - i) == 0 ? HIGH : LOW);
    }
}

void loop() {
    delayMicroseconds(STEP_DELAY);
    step(true);
}
