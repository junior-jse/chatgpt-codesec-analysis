#include <stdio.h>
#include <stdlib.h>

int* find_min_indices(const int* arr, size_t size, size_t* out_size) {
    if (arr == NULL || out_size == NULL || size == 0) {
        if (out_size != NULL) {
            *out_size = 0;
        }
        return NULL;
    }

    int min_val = arr[0];
    for (size_t i = 1; i < size; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }

    size_t count = 0;
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == min_val) {
            count++;
        }
    }

    int* indices = (int*)malloc(count * sizeof(int));
    if (indices == NULL) {
        *out_size = 0;
        return NULL;
    }

    size_t idx = 0;
    for (size_t i = 0; i < size; i++) {
        if (arr[i] == min_val) {
            indices[idx++] = (int)i;
        }
    }

    *out_size = count;
    return indices;
}

int main(void) {
    int arr[] = {4, 2, 7, 2, 5, 2, 9};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    size_t out_size = 0;

    int* indices = find_min_indices(arr, size, &out_size);

    if (indices != NULL) {
        for (size_t i = 0; i < out_size; i++) {
            printf("%d ", indices[i]);
        }
        printf("\n");
        free(indices);
    }

    return 0;
}