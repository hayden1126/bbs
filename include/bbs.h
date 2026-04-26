#ifndef BBS_H
#define BBS_H

#include <stdint.h>

typedef uint64_t Board;

typedef struct {
    uint64_t mask;
    int w;
    int h;
} Shape;

typedef struct {
    int   order[3];   /* shape index placed at step i */
    int   r[3];       /* r[i], c[i] is where shape order[i] was placed */
    int   c[3];
    int   score;      /* board_score(final); higher is better */
    Board final;
} Solution;

Shape shape_make(const int coords[]);
int   board_can_place(Board b, Shape s, int r, int c);
Board board_place(Board b, Shape s, int r, int c);
Board board_clear_lines(Board b, int *cleared);
int   board_score(Board b);
void  board_print(Board b);
int   solve(Board b, const Shape shapes[3], Solution *out);

#endif
