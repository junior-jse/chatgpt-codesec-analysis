#include <stdio.h>
#include <stdlib.h>

static int compare_ints(const void *left, const void *right)
{
    const int a = *(const int *)left;
    const int b = *(const int *)right;

    return (a > b) - (a < b);
}

static size_t rotations_to_sorted(const int *array, size_t length)
{
    size_t pivot = 0;

    if (array == NULL || length < 2) {
        return 0;
    }

    for (size_t i = 1; i < length; ++i) {
        if (array[i] < array[pivot]) {
            pivot = i;
        }
    }

    for (size_t i = 0; i + 1 < length; ++i) {
        if (array[(pivot + i) % length] > array[(pivot + i + 1) % length]) {
            return SIZE_MAX;
        }
    }

    return pivot;
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

    size_t rotations = rotations_to_sorted(array, length);
    if (rotations == SIZE_MAX) {
        free(array);
        return EXIT_FAILURE;
    }

    printf("%zu\n", rotations);

    free(array);
    return EXIT_SUCCESS;
}