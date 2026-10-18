#include <stdio.h>
#include <stdlib.h>

int find_max_frequency(const int *arr, size_t size, int *max_freq_item) {
    if (arr == NULL || max_freq_item == NULL || size == 0) {
        return -1;
    }

    int max_count = 0;
    *max_freq_item = arr[0];

    for (size_t i = 0; i < size; i++) {
        int count = 0;
        for (size_t j = 0; j < size; j++) {
            if (arr[i] == arr[j]) {
                count++;
            }
        }
        if (count > max_count) {
            max_count = count;
            *max_freq_item = arr[i];
        }
    }

    return 0;
}

int main(void) {
    int arr[] = {1, 3, 2, 3, 4, 3, 2, 2, 2, 5};
    size_t size = sizeof(arr) / sizeof(arr[0]);
    int max_freq_item;

    if (find_max_frequency(arr, size, &max_freq_item) == 0) {
        printf("Item with maximum frequency: %d\n", max_freq_item);
    } else {
        fprintf(stderr, "Error: Invalid input\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}