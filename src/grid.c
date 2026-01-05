
#include <stdio.h>
#include "grid.h"
#include "coordinate.h"

void print_shape(int shape[]) { // creates a grid representation of the shape
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