/* 
 * utils.c 
 */

#include <stdio.h>
#include "utils.h"

int findmin(int *arr, int size) {
    if (size <= 0) {
        return -1; // Indicate error for empty array
    }
    int min = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }
    return min;
}

int findmax(int *arr, int size) {
    if (size <= 0) {
        return -1; // Indicate error for empty array
    }
    int max = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}