#include "bbs.h"
#include <stdio.h>

#define MAX_LINE 64
#define MAX_ROWS 16

/* Read one block of non-blank lines from stdin into `lines`. Skips leading
   blanks; stops at the next blank line or EOF. Returns line count (0 on EOF). */
static int read_block(char lines[MAX_ROWS][MAX_LINE]) {
    int n = 0, started = 0;
    char scratch[MAX_LINE];
    for (;;) {
        char *target = (n < MAX_ROWS) ? lines[n] : scratch;
        if (!fgets(target, MAX_LINE, stdin)) break;
        int blank = (target[0] == '\n' || target[0] == '\r' || target[0] == 0);
        if (!started) {
            if (blank) continue;
            started = 1;
        } else if (blank) {
            break;
        }
        if (n < MAX_ROWS) n++;
    }
    return n;
}

int main(void) {
    char lines[MAX_ROWS][MAX_LINE];
    int n;

    n = read_block(lines);
    if (n != 8) {
        fprintf(stderr, "board: expected 8 lines, got %d\n", n);
        return 1;
    }
    Board b = 0;
    for (int r = 0; r < 8; r++)
        for (int c = 0; c < 8 && lines[r][c] && lines[r][c] != '\n' && lines[r][c] != '\r'; c++)
            if (lines[r][c] == '#') b |= 1ULL << (r * 8 + c);

    Shape shapes[3];
    for (int i = 0; i < 3; i++) {
        n = read_block(lines);
        if (n == 0) {
            fprintf(stderr, "shape %d: missing\n", i);
            return 1;
        }
        int coords[65];
        int k = 0;
        for (int r = 0; r < n && r < 8; r++)
            for (int c = 0; c < 8 && lines[r][c] && lines[r][c] != '\n' && lines[r][c] != '\r'; c++)
                if (lines[r][c] == '#' && k < 64) coords[k++] = r * 8 + c;
        coords[k] = -1;
        shapes[i] = shape_make(coords);
    }

    Solution sol;
    if (!solve(b, shapes, &sol)) {
        printf("No solution.\n");
        return 1;
    }

    printf("Score: %d\n", sol.score);
    for (int i = 0; i < 3; i++)
        printf("Step %d: shape %d at row %d, col %d\n",
               i + 1, sol.order[i], sol.r[i], sol.c[i]);
    printf("Final:\n");
    board_print(sol.final);
    return 0;
}
