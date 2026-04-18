# bbs — Block Blast Solver

A solver for [Block Blast](https://en.wikipedia.org/wiki/Block_Blast), written in C.

Block Blast is a puzzle game where the player is given three shapes each turn and must place them on an 8×8 grid. Rows or columns that become completely filled clear and free up space. The objective is to survive as long as possible without running out of room for the next shape.

## Architecture

The codebase is organized around three core data types, each in its own module:

| Module | Header | Source | Responsibility |
|---|---|---|---|
| `shape` | `include/shape.h` | `src/shape.c` | Represents a placeable piece |
| `coordinate` | `include/coordinate.h` | `src/coordinate.c` | 2D grid position primitive |
| `grid` | `include/grid.h` | `src/grid.c` | 8×8 board state and placement / clear logic |

## Build & run

```bash
make          # build the binary into bin/shape_test
make test     # run the binary
make check    # run and verify expected output
make clean    # remove build artefacts
```

Requires `gcc` with C11 support.

## Status

Core data structures and placement logic are in place. Search / solver logic is under active development.
