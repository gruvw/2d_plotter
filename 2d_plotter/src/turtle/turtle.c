#include "turtle.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "../draw/draw.h"
#include "../hardware/servo.h"
#include "../hardware/stepper.h"
#include "input.h"

#define MAX_INSTR_PER_LOOP 50
#define TURTLE_CIRCLE_PRECISION 8

#define FULL_ROT 360
#define WAY(d) (d == FORWARD ? 1 : -1)

typedef enum {
    PENUP,
    PENDOWN,
    FORWARD,
    BACKWARD,
    CIRCLE,
    HOME,
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
    Instruction * instructions;
    int program_counter;
} Program;

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
    if (strcmp(word, "backward") == 0) {
        return BACKWARD;
    }
    if (strcmp(word, "circle") == 0) {
        return CIRCLE;
    }
    if (strcmp(word, "home") == 0) {
        return HOME;
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

    // ignore token
}

Instruction get_next_instruction(Program * const program) {
    if (program != NULL) {
        return program->instructions[program->program_counter++];
    }

    char * const token_word = get_next_word();
    const Token token = tokenize(token_word);
    free(token_word);

    // Instructions without argument
    if (token == PENUP || token == PENDOWN || token == HOME || token == LOOP_START ||
        token == LOOP_END) {
        return (Instruction) {token};
    }

    char * const arg_word = get_next_word();
    const Instruction instr = {token, atoi(arg_word)};
    free(arg_word);

    return instr;
}

void process_instruction(Turtle * turtle, Instruction instr, Program * program) {
    Plotter * const plotter = turtle->plotter;

    if (instr.token == PENUP && turtle->is_pendown) {
        turtle->is_pendown = false;
        pen_up(plotter->pen);
    } else if (instr.token == PENDOWN && !turtle->is_pendown) {
        turtle->is_pendown = true;
        pen_down(plotter->pen);
    } else if (instr.token == FORWARD || instr.token == BACKWARD) {
        const double radians = TO_RAD(turtle->angle);
        const int way = WAY(instr.token);
        const int x = (int) (instr.arg * cos(radians)), y = (int) (instr.arg * sin(radians));
        draw_line_to(plotter->axes, plotter->axes->X.pos + x * way, plotter->axes->Y.pos + y * way);
    } else if (instr.token == CIRCLE) {
        draw_circle(plotter->axes, instr.arg, turtle->angle, TURTLE_CIRCLE_PRECISION);
    } else if (instr.token == HOME) {
        draw_line_to(plotter->axes, AREA_SIDE / 2, AREA_SIDE / 2);
    } else if (instr.token == LEFT) {
        turtle->angle -= instr.arg;
        turtle->angle = (turtle->angle + FULL_ROT) % FULL_ROT;
    } else if (instr.token == RIGHT) {
        turtle->angle += instr.arg;
        turtle->angle = turtle->angle % FULL_ROT;
    } else if (instr.token == REPEAT && get_next_instruction(program).token == LOOP_START) {
        // Fetch the body of the loop, including nested loops
        Instruction body[MAX_INSTR_PER_LOOP] = { 0 };
        int body_len = 0;
        for (int n_loops = 1; body_len < MAX_INSTR_PER_LOOP && n_loops > 0; ++body_len) {
            body[body_len] = get_next_instruction(program);
            n_loops += body[body_len].token == LOOP_START;
            n_loops -= body[body_len].token == LOOP_END;
        }

        // Execute loop body `instr.arg` times
        for (int iter = 0; iter < instr.arg; ++iter) {
            Program loop = { body, 0 };
            while (loop.program_counter < body_len) {
                process_instruction(turtle, get_next_instruction(&loop), &loop);
            }
        }
    }

    // ingnore instruction
}

void turtle_main(Turtle * const turtle) {
    while (1) {
        process_instruction(turtle, get_next_instruction(NULL), NULL);
    }
}

// Examlpes
// repeat 5 [ repeat 360 [ forward 10 right 1 ] right 72 ]
