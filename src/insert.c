/*
 * insert.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "insert.h"
#include "utils.h"
#include "coordinate.h"
#include "grid.h"

#define MAXWEIGHT 9

static void inserterror(const char *msg) {
    fprintf(stderr, "Insert Error: %s\n", msg);
}

struct InsertObject {
    int shape[MAXWEIGHT+1];
    size_t weight;
    size_t fwidth;
    size_t flength;
};

struct InsertObject* create_insert(int shape[]) {
    struct InsertObject* obj = (struct InsertObject*)calloc(1, sizeof(struct InsertObject));
    if (!obj) {
        inserterror("Memory allocation failed");
        return NULL;
    }

    int min_row = MAXWEIGHT;
    int min_col = MAXWEIGHT;
    int max_row = 0;
    int max_col = 0;

    // Iterate through each coordinate
    int index = 0;
    while (shape[index] > 0)
    {
        obj->weight++;
        int row = get_row(shape[index]);
        int col = get_col(shape[index]);

        if (row < min_row) min_row = row;
        if (col < min_col) min_col = col;
        if (row > max_row) max_row = row;
        if (col > max_col) max_col = col;
        index++;
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
    struct InsertObject* insert_obj = create_insert(test_shape);
    if (insert_obj) {
        printf("Insert Object created successfully.\n");
        printf("Weight: %zu\n", insert_obj->weight);
        printf("Frame Width: %zu\n", insert_obj->fwidth);
        printf("Frame Length: %zu\n", insert_obj->flength);
        print_shape(insert_obj->shape);
        free(insert_obj);
    } else {
        printf("Failed to create Insert Object.\n");
    }
    return 0;
}