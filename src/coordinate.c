#include <stdlib.h>
#include "coordinate.h"
#include <stdio.h>

/*
coordinates are stored as integers where the tens place is the column and the units place is the row.
They are zero-indexed.
For example, coordinate 23 represents row 3, column 2.
*/

int get_row(int coord) {
    return coord % 10;
}

int get_col(int coord) {
    return coord / 10;
}

int make_coord(int row, int col) {
    return col * 10 + row;
}