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
    SETHEADING,
    REPEAT,
    LOOP_START,
    LOOP_END,
    NO_OP,  // does nothing, must be last
} Token;

// In order correspondance between token and actual program text (NO_OP omitted as default)
const char * const tokens_text[] = {
    "penup",
    "pendown",
    "forward",
    "backward",
    "circle",
    "home",
    "right",
    "left",
    "setheading",
    "repeat",
    "[",
    "]",
};

// Tokens that does not have an argument following
const Token standalone_tokens[] = {
    PENUP,
    PENDOWN,
    HOME,
    LOOP_START,
    LOOP_END,
    NO_OP,  // do not fetch argument if unrecognized token
};

typedef struct {
    Token token;
    int arg;
} Instruction;

typedef struct {
    Instruction * instructions;
    int program_counter;
} Program;

Token tokenize(char * word) {
    for (int i = 0; i < NO_OP; ++i) {
        if (strcmp(word, tokens_text[i]) == 0) {
            return (Token) i;
        }
    }

    // ignore unrecognized tokens
    return NO_OP;
}

Instruction get_next_instruction(Program * const program) {
    if (program != NULL) {
        // Instruction already provided by `program`, do not consume standard input
        return program->instructions[program->program_counter++];
    }

    // Fetch and decode instructions from standard input
    char * const token_word = get_next_word();
    const Token token = tokenize(token_word);
    free(token_word);

    // Instructions without argument
    for (int i = 0; i < sizeof(standalone_tokens) / sizeof(*standalone_tokens); ++i) {
        if (token == standalone_tokens[i]) {
            return (Instruction) {token};
        }
    }

    // Fetch and parse argument from standard input
    char * const arg_word = get_next_word();
    const Instruction instr = {token, atoi(arg_word)};
    free(arg_word);

    return instr;
}

// Executes the given instructions `instr`;
// gets additional instructions (when needed) from `program` if provided, standard input otherwise
void process_instruction(Turtle * turtle, Instruction instr, Program * program) {
    Plotter * const plotter = turtle->plotter;

    if (instr.token == PENUP && turtle->is_pendown) {
        turtle->is_pendown = false;
        penup(plotter->pen);
    } else if (instr.token == PENDOWN && !turtle->is_pendown) {
        turtle->is_pendown = true;
        pendown(plotter->pen);
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
    } else if (instr.token == SETHEADING) {
        turtle->angle = (instr.arg + FULL_ROT) % FULL_ROT;
    } else if (instr.token == REPEAT && get_next_instruction(program).token == LOOP_START) {
        // Fetch the body of the loop, including nested loops
        Instruction body[MAX_INSTR_PER_LOOP] = {0};
        int body_len = 0;
        for (int n_loops = 1; body_len < MAX_INSTR_PER_LOOP && n_loops > 0; ++body_len) {
            body[body_len] = get_next_instruction(program);
            n_loops += body[body_len].token == LOOP_START;
            n_loops -= body[body_len].token == LOOP_END;
        }

        // Execute loop body `instr.arg` times
        for (int iter = 0; iter < instr.arg; ++iter) {
            Program loop = {body, 0};
            while (loop.program_counter < body_len) {
                process_instruction(turtle, get_next_instruction(&loop), &loop);
            }
        }
    }

    // ignore instruction
}

void turtle_main(Turtle * const turtle) {
    while (1) {
        process_instruction(turtle, get_next_instruction(NULL), NULL);
    }
}
