#include "realisation.h"
#include <stdio.h>
#include <stdlib.h>
#define check(VALUE, OKVAL, MSG) if (VALUE != OKVAL) { printf("%s", MSG); return 1; }

int main(int argc, const char** argv) {
    int q;
    printf("Enter query: 1) integral sin(x)dx    2) sort array\n");
    while (scanf("%d", &q) > 0) {
        if (q == 1) {
            float A, B, e;
            printf("enter  A, B, e: ");
            check(scanf("%f %f %f", &A, &B, &e), 3, "Error reading floats!\n")
            printf("Integral from %f to %f sin(x)dx = %f\n", A, B, SinIntegral(A, B, e));
        } else if (q == 2) {
            size_t size;
            int* array;
            printf("enter array size: ");
            check(scanf("%lu", &size), 1, "Error reading array size!\n")
            
            array = malloc((size + 1) * sizeof(int));
            if (array == NULL) {
                printf("Cannot allocate memmory\n");
                return 2;
            }
            array[0] = (int) size;

            printf("enter elements: ");
            for (int i = 1; i < size + 1; ++i) {
               check(scanf("%d", array + i), 1, "Error reading integer\n")
            }

            array = Sort(array);
            
            printf("Sorted array: ");
            for (int i = 1; i < size + 1; ++i) {
                printf("%d ", array[i]);
            }
            printf("\n");
            free(array);
        } else {
            printf("End.\n");
            return 0;
        }
    }
}

