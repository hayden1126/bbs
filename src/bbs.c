#include "bbs.h"
#include <stdio.h>

Shape shape_make(const int coords[]) {
    Shape s = {0, 0, 0};
    if (coords[0] < 0) return s;

    int min_r = 7, min_c = 7, max_r = 0, max_c = 0;
    for (int i = 0; coords[i] >= 0; i++) {
        int r = coords[i] / 8, c = coords[i] % 8;
        if (r < min_r) min_r = r;
        if (c < min_c) min_c = c;
        if (r > max_r) max_r = r;
        if (c > max_c) max_c = c;
    }
    for (int i = 0; coords[i] >= 0; i++) {
        int r = coords[i] / 8 - min_r;
        int c = coords[i] % 8 - min_c;
        s.mask |= 1ULL << (r * 8 + c);
    }
    s.h = max_r - min_r + 1;
    s.w = max_c - min_c + 1;
    return s;
}

int board_can_place(Board b, Shape s, int r, int c) {
    if (r < 0 || c < 0 || r + s.h > 8 || c + s.w > 8) return 0;
    return (b & (s.mask << (r * 8 + c))) == 0;
}

Board board_place(Board b, Shape s, int r, int c) {
    return b | (s.mask << (r * 8 + c));
}

Board board_clear_lines(Board b, int *cleared) {
    uint64_t to_clear = 0;
    int n = 0;
    for (int r = 0; r < 8; r++) {
        uint64_t row = 0xFFULL << (r * 8);
        if ((b & row) == row) { to_clear |= row; n++; }
    }
    for (int c = 0; c < 8; c++) {
        uint64_t col = 0x0101010101010101ULL << c;
        if ((b & col) == col) { to_clear |= col; n++; }
    }
    if (cleared) *cleared = n;
    return b & ~to_clear;
}

int board_score(Board b) {
    uint64_t e = ~b;
    uint64_t up    = e << 8;
    uint64_t down  = e >> 8;
    uint64_t left  = (e << 1) & 0xFEFEFEFEFEFEFEFEULL;
    uint64_t right = (e >> 1) & 0x7F7F7F7F7F7F7F7FULL;
    uint64_t iso   = e & ~(up | down | left | right);
    return __builtin_popcountll(e) - 2 * __builtin_popcountll(iso);
}

void board_print(Board b) {
    for (int r = 0; r < 8; r++) {
        for (int c = 0; c < 8; c++) {
            putchar((b & (1ULL << (r * 8 + c))) ? '#' : '.');
        }
        putchar('\n');
    }
}

int solve(Board b, const Shape shapes[3], Solution *out) {
    static const int perms[6][3] = {
        {0,1,2}, {0,2,1}, {1,0,2}, {1,2,0}, {2,0,1}, {2,1,0}
    };
    int best = -1;

    for (int p = 0; p < 6; p++) {
        const int *o = perms[p];
        Shape s0 = shapes[o[0]], s1 = shapes[o[1]], s2 = shapes[o[2]];

        for (int r0 = 0; r0 + s0.h <= 8; r0++)
        for (int c0 = 0; c0 + s0.w <= 8; c0++) {
            if (!board_can_place(b, s0, r0, c0)) continue;
            Board b1 = board_clear_lines(board_place(b, s0, r0, c0), NULL);

            for (int r1 = 0; r1 + s1.h <= 8; r1++)
            for (int c1 = 0; c1 + s1.w <= 8; c1++) {
                if (!board_can_place(b1, s1, r1, c1)) continue;
                Board b2 = board_clear_lines(board_place(b1, s1, r1, c1), NULL);

                for (int r2 = 0; r2 + s2.h <= 8; r2++)
                for (int c2 = 0; c2 + s2.w <= 8; c2++) {
                    if (!board_can_place(b2, s2, r2, c2)) continue;
                    Board b3 = board_clear_lines(board_place(b2, s2, r2, c2), NULL);

                    int score = board_score(b3);
                    if (score > best) {
                        best = score;
                        out->order[0] = o[0]; out->order[1] = o[1]; out->order[2] = o[2];
                        out->r[0] = r0; out->c[0] = c0;
                        out->r[1] = r1; out->c[1] = c1;
                        out->r[2] = r2; out->c[2] = c2;
                        out->score = score;
                        out->final = b3;
                    }
                }
            }
        }
    }
    return best >= 0;
}
