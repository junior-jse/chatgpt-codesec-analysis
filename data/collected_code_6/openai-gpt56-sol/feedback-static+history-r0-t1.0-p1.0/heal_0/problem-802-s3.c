#include <stdio.h>
#include <stdlib.h>

static size_t rotation_count(const int *array, size_t length)
{
    size_t left = 0;
    size_t right = length - 1;

    while (left < right) {
        size_t middle = left + (right - left) / 2;

        if (array[middle] < array[right]) {
            right = middle;
        } else if (array[middle] > array[right]) {
            left = middle + 1;
        } else {
            --right;
        }
    }

    return left;
}

int main(void)
{
    size_t length;

    if (scanf("%zu", &length) != 1 || length == 0 ||
        length > SIZE_MAX / sizeof(int)) {
        fputs("Invalid input\n", stderr);
        return EXIT_FAILURE;
    }

    int *array = malloc(length * sizeof(*array));
    if (array == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < length; ++i) {
        if (scanf("%d", &array[i]) != 1) {
            fputs("Invalid input\n", stderr);
            free(array);
            return EXIT_FAILURE;
        }
    }

    printf("%zu\n", rotation_count(array, length));

    free(array);
    return EXIT_SUCCESS;
}