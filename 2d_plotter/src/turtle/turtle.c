#include "turtle.h"

#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../draw/draw.h"
#include "../hardware/servo.h"
#include "../hardware/stepper.h"
#include "../input.h"

#define FULL_ROT 360
#define TO_RAD(x) ((x) *M_PI / 180)
#define MAX_INSTR_PER_LOOP 50

typedef enum {
    PENUP,
    PENDOWN,
    FORWARD,
    RIGHT,  // clockwise
    LEFT,  // counter-clockwise
    REPEAT,
    LOOP_START,
    LOOP_END,
} Token;

typedef struct {
    Token token;
    int arg;
} Instruction;

typedef struct {
    Axes2D * axes;
    Servo * pen;
    bool is_pendown;
    int angle;  // in degrees
} Turtle;

Token tokenize(char * word) {
    if (strcmp(word, "penup") == 0) {
        return PENUP;
    }
    if (strcmp(word, "pendown") == 0) {
        return PENDOWN;
    }
    if (strcmp(word, "forward") == 0) {
        return FORWARD;
    }
    if (strcmp(word, "left") == 0) {
        return LEFT;
    }
    if (strcmp(word, "right") == 0) {
        return RIGHT;
    }
    if (strcmp(word, "repeat") == 0) {
        return REPEAT;
    }
    if (strcmp(word, "[") == 0) {
        return LOOP_START;
    }
    if (strcmp(word, "]") == 0) {
        return LOOP_END;
    }

    exit(1);
}

Instruction get_next_instruction() {
    char * const token_word = get_next_word();
    const Token token = tokenize(token_word);
    free(token_word);

    // Instructions without argument
    if (token == PENUP || token == PENDOWN || token == LOOP_START || token == LOOP_END) {
        return (Instruction) {token};
    }

    char * const arg_word = get_next_word();
    const Instruction instr = {token, atoi(arg_word)};
    free(arg_word);

    return instr;
}

void process_instruction(Turtle * turtle, Instruction instr) {
    if (instr.token == PENUP && turtle->is_pendown) {
        turtle->is_pendown = false;
        pen_up(turtle->pen);
    } else if (instr.token == PENDOWN && !turtle->is_pendown) {
        turtle->is_pendown = true;
        pen_down(turtle->pen);
    } else if (instr.token == S_FORWARD) {
        const double radians = TO_RAD(turtle->angle);
        const int x = (int) (instr.arg * cos(radians)), y = (int) (instr.arg * sin(radians));
        draw_line_to(turtle->axes, turtle->axes->X.pos + x, turtle->axes->Y.pos + y);
    } else if (instr.token == LEFT) {
        turtle->angle -= (instr.arg + FULL_ROT) % FULL_ROT;
    } else if (instr.token == RIGHT) {
        turtle->angle += instr.arg % FULL_ROT;
    } else if (instr.token == REPEAT && get_next_instruction().token == LOOP_START) {
        // Fetch the body of the loop, including nested loops
        Instruction body[MAX_INSTR_PER_LOOP] = { 0 };
        int body_len = 0;
        for (int n_loops = 1; body_len < MAX_INSTR_PER_LOOP && n_loops > 0; ++body_len) {
            body[body_len] = get_next_instruction();
            n_loops += body[body_len].token == LOOP_START;
            n_loops -= body[body_len].token == LOOP_END;
        }

        // LEFT HERE

        // Execute loop body `instr.arg` times
        for (int iter = 0; iter < instr.arg; ++iter) {
            for (int in = 0; body[in].token != LOOP_END; ++in) {  // BUG
                process_instruction(turtle, body[in]);
            }
        }
    } else {
        exit(1);
    }
}
