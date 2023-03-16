#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#define check(VALUE, OKVAL, MSG) if (VALUE != OKVAL) { printf("%s", MSG); return 1; }
#define check_wrong(VALUE, WRONG_VAL, MSG) if (VALUE == WRONG_VAL) { printf("%s", MSG); return 1; }

// it is very important to make prefix "lib" and set extension ".so"
const char* DYN_LIB_1 = "./libDyn1.so";
const char* DYN_LIB_2 = "./libDyn2.so";

const char* SIN_INTEGRAL_NAME = "SinIntegral";
const char* SORT_NAME = "Sort";

int main(int argc, const char** argv) {
    int dynLibNum = 1;
    void* handle = dlopen(DYN_LIB_1, RTLD_LAZY);
    check_wrong(handle, NULL, "Error opening dynamic library!\n")
    float (*Integral)(float, float, float);
    int* (*Sort)(int*);
    Integral = dlsym(handle, SIN_INTEGRAL_NAME);
    Sort = dlsym(handle, SORT_NAME);
    char* error = dlerror();
    check(error, NULL, error)
    int q;
    size_t size;
    int* array;
    float A, B, e;
    printf("Enter query: 0) change realisation    1) integral sin(x)dx     2) sort array\n");
    while (scanf("%d", &q) > 0) {
        switch (q) {
            case 0:
                check(dlclose(handle), 0, "Error closing dynamic library!\n")
                if (dynLibNum) {
                    handle = dlopen(DYN_LIB_2, RTLD_LAZY);
                } else {
                    handle = dlopen(DYN_LIB_1, RTLD_LAZY);
                }
                check_wrong(handle, NULL, "Error opening dynamic library!\n")
                Integral = dlsym(handle, SIN_INTEGRAL_NAME);
                Sort = dlsym(handle, SORT_NAME);
                error = dlerror();
                check(error, NULL, error)
                /* switch between 0 and 1 */
                dynLibNum = dynLibNum ^ 1;
                break;
            case 1:
                printf("enter  A, B, e: ");
                check(scanf("%f %f %f", &A, &B, &e), 3, "Error reading floats!\n")
                printf("Integral from %f to %f sin(x)dx = %f\n", A, B, Integral(A, B, e));
                break;
            case 2:
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
                break;
            default:
                printf("End.\n");
                check(dlclose(handle), 0, "Error closing dynamic library!\n")
                return 0;
        }
    }
}
