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

struct Grid *create_grid() {
    struct Grid *grid = malloc(sizeof(struct Grid));
    if (!grid) {
        griderror("Memory allocation failed");
        return NULL;
    }

    grid->weight = 0;
    grid->shape = create_shape((int[]){-1}); // Initialize with empty shape
    if (!grid->shape) {
        free(grid);
        return NULL;
    }

    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            grid->orthogonal_neighbor_counts[i][j] = 4;
            if (i == 0 || i == GRID_ROWS - 1) grid->orthogonal_neighbor_counts[i][j]--;
            if (j == 0 || j == GRID_COLS - 1) grid->orthogonal_neighbor_counts[i][j]--;
        }
    }
    return grid;
}

void insert_shape(struct Grid *grid, struct Shape *shape_obj) {
    if (!grid) {
        griderror("Grid is NULL");
        return;
    }

    if (grid->weight + shape_obj->weight > MAXWEIGHT) {
        griderror("Shape exceeds grid capacity");
        return;
    }

    // Insert the shape into the grid
    for (size_t i = 0; i < shape_obj->weight; i++) {
        if (grid->weight + i >= MAXWEIGHT) {
            griderror("Coords array out of bounds");
            return;
        }
        grid->shape->coords[grid->weight + i] = shape_obj->coords[i];

        int row = get_row(shape_obj->coords[i]);
        int col = get_col(shape_obj->coords[i]);
        // Update orthogonal neighbor counts
        if (row > 0) grid->orthogonal_neighbor_counts[row - 1][col]--;
        if (row < GRID_ROWS - 1) grid->orthogonal_neighbor_counts[row + 1][col]--;
        if (col > 0) grid->orthogonal_neighbor_counts[row][col - 1]--;
        if (col < GRID_COLS - 1) grid->orthogonal_neighbor_counts[row][col + 1]--;  

    }
    grid->weight += shape_obj->weight;
    grid->shape->coords[grid->weight] = -1; // Null-terminate
}

void preview(int coords[]) { // creates a grid representation of the shape
    int max_row = -1;
    int max_col = -1;

    // First, determine the size of the grid
    for (int i = 0; coords[i] >= 0; i++) {
        int row = get_row(coords[i]);
        int col = get_col(coords[i]);
        if (row > max_row) max_row = row;
        if (col > max_col) max_col = col;
    }

    if (max_row < 0 || max_col < 0) {
        griderror("No valid coordinates provided for preview");
        return;
    }

    // Create and initialize the grid
    char grid[max_row][max_col];
    for (int r = 0; r < max_row; r++) {
        for (int c = 0; c < max_col; c++) {
            grid[r][c] = '.';
        }
    }

    // Mark the shape positions in the grid
    for (int i = 0; coords[i] >= 0; i++) {
        int row = get_row(coords[i]);
        int col = get_col(coords[i]);
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