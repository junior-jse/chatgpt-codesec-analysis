#include <stdio.h>
#include <stdlib.h>

static size_t rotation_count(const int *array, size_t length)
{
    size_t low = 0;
    size_t high = length - 1;

    while (low < high) {
        size_t middle = low + (high - low) / 2;

        if (array[middle] < array[high]) {
            high = middle;
        } else if (array[middle] > array[high]) {
            low = middle + 1;
        } else {
            --high;
        }
    }

    return low;
}

int main(void)
{
    size_t length;

    if (scanf("%zu", &length) != 1 || length == 0 ||
        length > SIZE_MAX / sizeof(int)) {
        return EXIT_FAILURE;
    }

    int *array = malloc(length * sizeof(*array));
    if (array == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            free(array);
            return EXIT_FAILURE;
        }
    }

    size_t result = rotation_count(array, length);

    if (printf("%zu\n", result) < 0) {
        free(array);
        return EXIT_FAILURE;
    }

    free(array);
    return EXIT_SUCCESS;
}