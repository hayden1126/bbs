#include "bbs.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
    /* 2x2 square, normalized regardless of input origin */
    Shape sq = shape_make((int[]){18, 19, 26, 27, -1});  /* (2,2)..(3,3) */
    assert(sq.w == 2 && sq.h == 2);
    assert(sq.mask == 0x0303);  /* bits 0,1,8,9 */

    /* placement and collision */
    Board b = 0;
    assert(board_can_place(b, sq, 0, 0));
    b = board_place(b, sq, 0, 0);
    assert(!board_can_place(b, sq, 0, 0));
    assert(!board_can_place(b, sq, 0, 1));
    assert( board_can_place(b, sq, 0, 2));

    /* out-of-bounds */
    assert(!board_can_place(b, sq, 7, 7));
    assert(!board_can_place(b, sq, -1, 0));

    /* fill row 0, then clear it */
    Shape six = shape_make((int[]){0, 1, 2, 3, 4, 5, -1});
    Shape two = shape_make((int[]){0, 1, -1});
    Board r = 0;
    r = board_place(r, six, 0, 0);
    r = board_place(r, two, 0, 6);
    int cleared = 0;
    r = board_clear_lines(r, &cleared);
    assert(cleared == 1);
    assert(r == 0);

    /* fill column 0, then clear it */
    Shape one = shape_make((int[]){0, -1});
    Board c = 0;
    for (int i = 0; i < 8; i++) c = board_place(c, one, i, 0);
    c = board_clear_lines(c, &cleared);
    assert(cleared == 1);
    assert(c == 0);

    /* no-op clear */
    Board n = board_place(0, sq, 0, 0);
    Board m = board_clear_lines(n, &cleared);
    assert(cleared == 0);
    assert(m == n);

    /* board_score: empty and full edge cases */
    assert(board_score(0)      == 64);
    assert(board_score(~0ULL)  == 0);

    /* board_score: a single empty cell at (4,4) surrounded by filled cells
       counts as isolated -> 1 empty - 2*1 isolated = -1 */
    Board hole = ~(1ULL << (4 * 8 + 4));
    assert(board_score(hole) == -1);

    /* board_score: two adjacent empty cells are not isolated */
    Board pair_hole = ~((1ULL << (4 * 8 + 4)) | (1ULL << (4 * 8 + 5)));
    assert(board_score(pair_hole) == 2);

    /* solver: empty board + three 2x2 squares fits trivially */
    Shape shapes[3] = {sq, sq, sq};
    Solution sol;
    assert(solve(0, shapes, &sol));
    assert(sol.score == 64 - 12);

    /* solver: prefers row-clearing placement.
       Pre-fill row 0 cols 0..5; offer a 2-wide piece (clears row 0)
       plus two single cells. Best score should leave only the two singles. */
    Board pre = 0;
    for (int i = 0; i < 6; i++) pre |= 1ULL << i;
    Shape pair = shape_make((int[]){0, 1, -1});
    Shape dot  = shape_make((int[]){0, -1});
    Shape three[3] = {pair, dot, dot};
    assert(solve(pre, three, &sol));
    assert(sol.score == 62);  /* row 0 cleared, 2 singles remain */

    /* solver: no fit -> returns 0 */
    Board full = ~0ULL;
    assert(!solve(full, three, &sol));

    printf("All tests passed.\n");
    return 0;
}
