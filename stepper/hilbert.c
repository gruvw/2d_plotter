#include "hilbert.h"

#include "function.h"

enum A {
  UP,
  LEFT,
  DOWN,
  RIGHT,
};

int pos_x = 0;
int pos_y = 0;
int dist = 0;
Axes2D * axess;

void move(enum A a) {
    if (a == UP) {
        pos_y -= dist;
    } else if (a == LEFT) {
        pos_x -= dist;
    } else if (a == DOWN) {
        pos_y += dist;
    } else if (a == RIGHT) {
        pos_x += dist;
    }

    move_line_to(axess, pos_x, pos_y);
}

void hilbert_level(int level,int direction)
{
  if (level==1) {
    switch (direction) {
    case LEFT:
      move(RIGHT);      /* move() could draw a line in... */
      move(DOWN);       /* ...the indicated direction */
      move(LEFT);
      break;
    case RIGHT:
      move(LEFT);
      move(UP);
      move(RIGHT);
      break;
    case UP:
      move(DOWN);
      move(RIGHT);
      move(UP);
      break;
    case DOWN:
      move(UP);
      move(LEFT);
      move(DOWN);
      break;
    } /* switch */
  } else {
    switch (direction) {
    case LEFT:
      hilbert_level(level-1,UP);
      move(RIGHT);
      hilbert_level(level-1,LEFT);
      move(DOWN);
      hilbert_level(level-1,LEFT);
      move(LEFT);
      hilbert_level(level-1,DOWN);
      break;
    case RIGHT:
      hilbert_level(level-1,DOWN);
      move(LEFT);
      hilbert_level(level-1,RIGHT);
      move(UP);
      hilbert_level(level-1,RIGHT);
      move(RIGHT);
      hilbert_level(level-1,UP);
      break;
    case UP:
      hilbert_level(level-1,LEFT);
      move(DOWN);
      hilbert_level(level-1,UP);
      move(RIGHT);
      hilbert_level(level-1,UP);
      move(UP);
      hilbert_level(level-1,RIGHT);
      break;
    case DOWN:
      hilbert_level(level-1,RIGHT);
      move(UP);
      hilbert_level(level-1,DOWN);
      move(LEFT);
      hilbert_level(level-1,DOWN);
      move(DOWN);
      hilbert_level(level-1,LEFT);
      break;
    } /* switch */
  } /* if */
}

void hilbert(Axes2D * axes2) {
    int level = 4;
    axess = axes2;
    dist = AREA_SIDE / pow(2, level);
    hilbert_level(level, UP);
}
