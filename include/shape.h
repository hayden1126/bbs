#ifndef SHAPE_H
#define SHAPE_H

#include <stddef.h>

#define MAXWEIGHT 8*8

struct Shape {
    int coords[MAXWEIGHT+1];
    size_t weight;
    size_t fwidth;
    size_t flength;
};

struct Shape* create_shape(int coords[]);
void free_shape(struct Shape* shape_obj);

#endif // SHAPE_H