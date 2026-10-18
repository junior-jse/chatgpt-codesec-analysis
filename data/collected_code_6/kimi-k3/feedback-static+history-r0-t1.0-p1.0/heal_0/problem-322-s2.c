#include <stdio.h>
#include <stdlib.h>

int *find_min_indices(const int *arr, int size, int *result_count) {
    if (arr == NULL || size <= 0 || result_count == NULL) {
        return NULL;
    }

    int min_val = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }

    int count = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == min_val) {
            count++;
        }
    }

    int *indices = (int *)malloc((size_t)count * sizeof(int));
    if (indices == NULL) {
        return NULL;
    }

    int j = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == min_val) {
            indices[j++] = i;
        }
    }

    *result_count = count;
    return indices;
}

int main(void) {
    int arr[] = {4, 2, 7, 2, 9, 2, 5};
    int size = (int)(sizeof(arr) / sizeof(arr[0]));
    int result_count = 0;

    int *indices = find_min_indices(arr, size, &result_count);

    if (indices == NULL) {
        fprintf(stderr, "Error: failed to find minimum indices\n");
        return EXIT_FAILURE;
    }

    printf("Minimum value indices: ");
    for (int i = 0; i < result_count; i++) {
        printf("%d", indices[i]);
        if (i < result_count - 1) {
            printf(", ");
        }
    }
    printf("\n");

    free(indices);
    indices = NULL;

    return EXIT_SUCCESS;
}