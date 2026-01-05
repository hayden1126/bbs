/*
 * insert.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "shape.h"
#include "coordinate.h"
#include "grid.h"

#define MAXWEIGHT 9

static void shapeerror(const char *msg) {
    fprintf(stderr, "Shape Error: %s\n", msg);
}

struct Shape {
    int shape[MAXWEIGHT+1];
    size_t weight;
    size_t fwidth;
    size_t flength;
};

struct Shape* create_shape(int shape[]) {
    struct Shape* obj = (struct Shape*)calloc(1, sizeof(struct Shape));
    if (!obj) {
        shapeerror("Memory allocation failed");
        return NULL;
    }

    int min_row = MAXWEIGHT;
    int min_col = MAXWEIGHT;
    int max_row = 0;
    int max_col = 0;

    // Iterate through each coordinate
    for (int i = 0; shape[i] > 0; i++)
    {
        obj->weight++;
        int row = get_row(shape[i]);
        int col = get_col(shape[i]);

        if (row < min_row) min_row = row;
        if (col < min_col) min_col = col;
        if (row > max_row) max_row = row;
        if (col > max_col) max_col = col;
    }

    obj->flength = max_row - min_row + 1;
    obj->fwidth = max_col - min_col + 1;

    // Parse shape into normalized form (1-indexed)
    for (size_t i = 0; i < obj->weight; i++) {
        int row = get_row(shape[i]) - min_row + 1;
        int col = get_col(shape[i]) - min_col + 1;
        obj->shape[i] = make_coord(row, col);
    }
    obj->shape[obj->weight] = 0; // Null-terminate

    return obj;
}

int main(void) {
    //test create_insert
    int test_shape[] = {11, 13, 33, 22, 0};
    struct Shape* shape_obj = create_shape(test_shape);
    if (shape_obj) {
        printf("Shape Object created successfully.\n");
        printf("Weight: %zu\n", shape_obj->weight);
        printf("Frame Width: %zu\n", shape_obj->fwidth);
        printf("Frame Length: %zu\n", shape_obj->flength);
        preview(shape_obj->shape);
        free(shape_obj);
    } else {
        printf("Failed to create Shape Object.\n");
    }
    return 0;
}