#ifndef GRID_H
#define GRID_H

#include "shape.h"

#define GRID_ROWS 8
#define GRID_COLS 8

struct Grid {
    struct Shape *shape;
    int orthogonal_neighbor_counts[GRID_ROWS][GRID_COLS];
    size_t weight;
};

void preview(int shape[]);
struct Grid *create_grid();
void insert_shape(struct Grid *grid, struct Shape *shape_obj);
int collision_check(struct Grid *grid, struct Shape *shape_obj);

#endif // GRID_H
