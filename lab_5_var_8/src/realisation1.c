#include "realisation.h"
#include <math.h>
#include <stdio.h>

void swap_int(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

float SinIntegral(float a, float b, float e) {
    float square = 0;
    for (float i = a; i <= b; i += e) {
        square += e * sin(i);
    }
    return square;
}


int* Sort(int* array) {
    int size = array[0];
    array++;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size - 1; ++j) {
            if (array[j] > array[j + 1]) {
                swap_int(array + j, array + j + 1);
            }
        }
    }
    return --array;
}

