#include <stdio.h>
#include <stdlib.h>

int maximum(int array[], size_t size) {
    int max = array[0];
    for (size_t i = 1; i < size; i++) {
        if (array[i] > max) {
            max = array[i];
        }
    }
    return max;
}

void counting_sort(int array[], size_t size, int place) {
    int *output = (int *)malloc(size * sizeof(int));
    if (output == NULL) {
        exit(EXIT_FAILURE);
    }

    int count[10] = {0};

    for (size_t i = 0; i < size; i++) {
        int index = (array[i] / place) % 10;
        count[index]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (size_t i = size; i > 0; i--) {
        int index = (array[i - 1] / place) % 10;
        output[count[index] - 1] = array[i - 1];
        count[index]--;
    }

    for (size_t i = 0; i < size; i++) {
        array[i] = output[i];
    }

    free(output);
}

void radix_sort(int array[], size_t size) {
    if (size < 2) {
        return;
    }

    int max = maximum(array, size);

    for (int place = 1; max / place > 0; place *= 10) {
        counting_sort(array, size, place);
    }
}

int main(void) {
    int array[] = {121, 432, 564, 23, 1, 45, 788};
    size_t size = sizeof(array) / sizeof(array[0]);

    radix_sort(array, size);

    for (size_t i = 0; i < size; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}