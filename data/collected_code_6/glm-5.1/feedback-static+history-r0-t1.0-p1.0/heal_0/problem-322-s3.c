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
    int list[] = {4, 2, 7, 1, 5, 1, 8, 1, 3};
    size_t size = sizeof(list) / sizeof(list[0]);
    size_t min_count = 0;

    int* min_indices = find_min_indices(list, size, &min_count);

    if (min_indices != NULL && min_count > 0) {
        printf("Minimum value found at indices: ");
        for (size_t i = 0; i < min_count; i++) {
            printf("%d ", min_indices[i]);
        }
        printf("\n");
        free(min_indices);
    } else {
        printf("No minimum indices found or allocation failed.\n");
    }

    return 0;
}