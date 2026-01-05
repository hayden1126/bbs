
#include <stdio.h>
#include <stdlib.h>
#include "grid.h"
#include "shape.h"
#include "coordinate.h"

#define GRID_ROWS 8
#define GRID_COLS 8

static void griderror(const char *msg) {
    fprintf(stderr, "Grid Error: %s\n", msg);
}

struct Grid {
    int shape[GRID_COLS * GRID_ROWS + 1];
    size_t weight;
};

struct Grid *create_grid() {
    struct Grid *grid = (struct Grid *)calloc(1, sizeof(struct Grid));
    if (!grid) {
        griderror("Memory allocation failed");
        return NULL;
    }
    return grid;
}

void insert_shape(struct Grid *grid, struct Shape *shape_obj) { // inserts a shape into the grid assuming it fits
    if (!grid) {
        griderror("Grid is NULL");
        return;
    }

    // Insert the shape into the grid
    for (size_t i = 0; i < shape_obj->weight; i++) {
        grid->shape[i] = shape_obj->shape[i];
    }
    grid->shape[shape_obj->weight] = 0; // Null-terminate
    grid->weight += shape_obj->weight;
}

int collision_check(struct Grid *grid, struct Shape *shape_obj) { // checks if a shape collides with existing shapes in the grid
    if (!grid) {
        griderror("Grid is NULL");
        return 1; // Indicate collision due to error
    }

    for (size_t i = 0; i < shape_obj->weight; i++) {
        int shape_coord = shape_obj->shape[i];
        for (size_t j = 0; j < grid->weight; j++) {
            if (shape_coord == grid->shape[j]) {
                return 1; // Collision detected
            }
        }
    }
    return 0; // No collision
}

void preview(int shape[]) { // creates a grid representation of the shape
    int max_row = 0;
    int max_col = 0;

    // First, determine the size of the grid
    for (int i = 0; shape[i] > 0; i++) {
        int row = get_row(shape[i]);
        int col = get_col(shape[i]);
        if (row > max_row) max_row = row;
        if (col > max_col) max_col = col;
    }

    // Create and initialize the grid
    char grid[max_row][max_col];
    for (int r = 0; r < max_row; r++) {
        for (int c = 0; c < max_col; c++) {
            grid[r][c] = '.';
        }
    }

    // Mark the shape positions in the grid
    for (int i = 0; shape[i] > 0; i++) {
        int row = get_row(shape[i]) - 1; // Convert to 0-indexed
        int col = get_col(shape[i]) - 1; // Convert to 0-indexed
        grid[row][col] = '#';
    }

    // Print the grid
    for (int r = 0; r < max_row; r++) {
        for (int c = 0; c < max_col; c++) {
            putchar(grid[r][c]);
        }
        putchar('\n');
    }
}