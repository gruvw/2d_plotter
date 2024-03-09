typedef struct {
    const int PIN;
    int pos;
} Servo;

Servo servo_setup();
void pen_up(Servo * servo);
void pen_down(Servo * servo);
