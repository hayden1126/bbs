# bbs — Block Blast Solver

A solver for [Block Blast](https://en.wikipedia.org/wiki/Block_Blast), written in C.

Block Blast is a puzzle game where the player is given three shapes each turn and must place them on an 8×8 grid. Rows or columns that become completely filled clear and free up space. The objective is to survive as long as possible without running out of room for the next shape.

## Architecture

The board is a `uint64_t` bitboard, with bit `r*8 + c` representing cell `(r, c)`. A shape is a normalized bitboard mask plus its bounding-box width and height; placing a shape is `board | (mask << (r*8 + c))`, and collision is `(board & shifted) != 0`. Full-line clears mask out completed rows (`0xFF << r*8`) and columns (`0x0101010101010101 << c`) in a single pass.

The whole module is in `include/bbs.h` and `src/bbs.c`; `src/tests.c` holds assertion-based tests, and `src/cli.c` is a stdin-driven front-end.

## Build & run

```bash
make          # build bin/bbs (CLI) and bin/bbs_test (assertions)
make test     # run the test binary
make check    # verify "All tests passed." was printed
make clean    # remove build artefacts
```

Requires `gcc` with C11 support.

## CLI

`bin/bbs` reads from stdin: 8 lines of `#`/`.` for the board, a blank line, then three shape blocks separated by blank lines. It prints the score, the placement order, and the final board. Example:

```
$ printf '########\n########\n........\n........\n........\n........\n........\n......##\n\n##\n##\n\n#\n\n#\n' | bin/bbs
Score: 56
Step 1: shape 0 at row 2, col 0
Step 2: shape 1 at row 0, col 0
Step 3: shape 2 at row 0, col 1
Final:
##......
........
##......
##......
........
........
........
......##
```

## Status

Board representation, placement, collision, line-clear, a brute-force three-piece solver, and a stdin CLI are in place and tested. Leaves are scored as `empty_cells − 2·isolated_empty_cells`, where an isolated empty has no empty orthogonal on-board neighbour.

## Next steps

In rough priority order. Each is independent — pick one off the list rather than doing them all.

- **Multi-turn driver.** Extend `cli.c` to loop: read N triplets of shapes from stdin, thread the board state forward across turns, print cumulative score. Lets the evaluator be exercised on real game traces instead of one-shot inputs.
- **Tune the isolation weight.** The `-2` in `board_score` is a guess. Make the multiplier configurable (`argv` or env var) and compare scores across a recorded corpus to pick a defensible value.
- **Near-clear bonus.** A row or column with 6–7 filled cells is one piece away from clearing; the score should reward leaving them. Cheap to compute: `popcount(b & row_mask)` per row.
- **Fragmentation count.** Number of distinct connected empty regions is a better signal than the raw isolated-cell count. Flood-fill on the empty bitboard, or repeated bit-tricks (`x & -x` to peel a region).
- **Precompute placement masks.** For each shape, materialise the list of valid `(r, c)` shifted masks once. Solver then iterates a flat array instead of recomputing shifts and bounds in the inner loop. Only worth doing if profiling says the inner loop matters.
- **Regression corpus.** A handful of board+shape inputs with expected outputs replayed under `make check` would catch evaluator regressions whenever the score weights are touched.
