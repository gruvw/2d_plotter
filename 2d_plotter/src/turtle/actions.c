#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define FULL_ROT 360
#define MAX_INSTR_PER_LOOP 50

// Assume you have the following methods already defined
void move_to(int x, int y);
void penup();
void pendown();
char * get_next_word();

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
    int x;
    int y;
    bool is_pendown;
    int angle;
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
    Token token = tokenize(get_next_word());

    // Instructions without argument
    if (token == PENUP || token == PENDOWN || token == LOOP_START || token == LOOP_END) {
        return (Instruction) { token };
    }

    return (Instruction) { token, atoi(get_next_word()) };
}

void process_instruction(Turtle * turtle, Instruction instr) {
    if (instr.token == PENUP && turtle->is_pendown) {
        turtle->is_pendown = false;
        penup();
    } else if (instr.token == PENDOWN && !turtle->is_pendown) {
        turtle->is_pendown = true;
        pendown();
    } else if (instr.token == FORWARD) {
        // TODO move in correct direction
    } else if (instr.token == LEFT) {
        turtle->angle -= (instr.arg + FULL_ROT) % FULL_ROT;
    } else if (instr.token == RIGHT) {
        turtle->angle += instr.arg % FULL_ROT;
    } else if (instr.token == REPEAT && get_next_instruction().token == LOOP_START) {
        // Fetch the body of the loop
        Instruction body[MAX_INSTR_PER_LOOP] = { 0 };
        for (int in = 0; in < MAX_INSTR_PER_LOOP && body[in].token != LOOP_END; ++in) {
            body[in] = get_next_instruction();
        }

        // Execute loop body
        for (int iter = 0; iter < instr.arg; ++iter) {
            for (int in = 0; body[in].token != LOOP_END; ++in) {
                process_instruction(turtle, body[in]);
            }
        }
    } else {
        exit(1);
    }
}
