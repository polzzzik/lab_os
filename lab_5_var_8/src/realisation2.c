#include "realisation.h"
#include "math.h"

void swap_int(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

float SinIntegral(float A, float B, float e){
    float square = 0;
    for (float i = A; i < B; i += e) {
        square += e * ((sin(i) + sin(i + e)) / 2);
    }
    return square;
}

void QuickSort(int* array, int l, int r) {
	if (l + 1 >= r) {
		return;
	}
	int i = l, j = r - 2;
	while (i < j) {
		if (array[i] < array[r - 1]) {
			++i;
		} else if (array[j] > array[r - 1]) {
			--j;
		} else {
			swap_int(&array[i], &array[j]);
			++i;
			--j;
		}
	}
	if (array[i] < array[r - 1]) {
		++i;
	}
	swap_int(array + i, array + r - 1);
	QuickSort(array, l, i);
	QuickSort(array, i + 1, r);
}

int* Sort(int* array) {
    int size = array[0];
    array++;
	QuickSort(array, 0, size);
    return --array;
}

