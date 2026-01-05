#ifndef SHAPE_H
#define SHAPE_H
#define MAXWEIGHT 9

struct Shape {
    int shape[MAXWEIGHT+1];
    size_t weight;
    size_t fwidth;
    size_t flength;
};


struct Shape* create_shape(int shape[]);

#endif // SHAPE_H