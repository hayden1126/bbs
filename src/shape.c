/*
 * insert.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "shape.h"
#include "coordinate.h"
#include "grid.h"

#define MAXWEIGHT 8*8

static void shapeerror(const char *msg) {
    fprintf(stderr, "Shape Error: %s\n", msg);
}

struct Shape* create_shape(int coords[]) {
    struct Shape *obj = malloc(sizeof(struct Shape));
    if (!obj) {
        shapeerror("Memory allocation failed");
        return NULL;
    }
    obj->weight = 0;
    obj->fwidth = 0;
    obj->flength = 0;

    // Initialize coords array to -1
    for (int i = 0; i <= MAXWEIGHT; i++) { 
        obj->coords[i] = -1;
    }

    // Check for empty input
    if (coords[0] == -1) {
        return obj;
    }

    int min_row = MAXWEIGHT;
    int min_col = MAXWEIGHT;
    int max_row = 0;
    int max_col = 0;


    // Iterate through each coordinate
    for (int i = 0; coords[i] >= 0; i++) {
        if (obj->weight >= MAXWEIGHT) {
            shapeerror("Too many coordinates");
            free(obj);
            return NULL;
        }

        obj->weight++;
        int row = get_row(coords[i]);
        int col = get_col(coords[i]);

        if (row < min_row) min_row = row;
        if (col < min_col) min_col = col;
        if (row > max_row) max_row = row;
        if (col > max_col) max_col = col;
    }

    obj->flength = max_row - min_row + 1;
    obj->fwidth = max_col - min_col + 1;

    // Parse coords into normalized form (0-indexed)
    for (size_t i = 0; i < obj->weight; i++) {
        int row = get_row(coords[i]) - min_row;
        int col = get_col(coords[i]) - min_col;
        obj->coords[i] = make_coord(row, col);
    }
    obj->coords[obj->weight] = -1; // Null-terminate
    
    return obj;
}

void free_shape(struct Shape* shape_obj) {
    if (shape_obj) {
        free(shape_obj);
    }
}
