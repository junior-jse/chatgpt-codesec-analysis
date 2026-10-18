#include <stdio.h>
#include <stdlib.h>

int* find_min_indices(const int *arr, int size, int *count) {
    if (arr == NULL || size <= 0 || count == NULL) {
        *count = 0;
        return NULL;
    }

    int min = arr[0];
    int min_count = 1;

    for (int i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
            min_count = 1;
        } else if (arr[i] == min) {
            min_count++;
        }
    }

    int *indices = (int*)malloc(min_count * sizeof(int));
    if (indices == NULL) {
        *count = 0;
        return NULL;
    }

    int index = 0;
    for (int i = 0; i < size; i++) {
        if (arr[i] == min) {
            indices[index++] = i;
        }
    }

    *count = min_count;
    return indices;
}

int main(void) {
    int arr[] = {5, 2, 8, 2, 9, 2, 1};
    int size = sizeof(arr) / sizeof(arr[0]);
    int count = 0;

    int *indices = find_min_indices(arr, size, &count);

    if (indices != NULL) {
        printf("Minimum value indices: ");
        for (int i = 0; i < count; i++) {
            printf("%d", indices[i]);
            if (i < count - 1) {
                printf(", ");
            }
        }
        printf("\n");
        free(indices);
    } else {
        printf("No minimum indices found or allocation failed.\n");
    }

    return 0;
}