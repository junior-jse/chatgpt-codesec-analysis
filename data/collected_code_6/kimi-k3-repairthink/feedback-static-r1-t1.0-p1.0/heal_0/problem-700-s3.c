#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

int count_in_range(const int *array, size_t length, int min, int max, size_t *result)
{
    size_t i;
    size_t count = 0;

    if (array == NULL || result == NULL) {
        return -1;
    }

    if (min > max) {
        return -2;
    }

    for (i = 0; i < length; i++) {
        if (array[i] >= min && array[i] <= max) {
            count++;
        }
    }

    *result = count;
    return 0;
}

int main(void)
{
    int test_array[] = {5, 12, 3, 8, 15, 7, 20, 1, 9, 11};
    size_t array_size = sizeof(test_array) / sizeof(test_array[0]);
    size_t count = 0;
    int status;

    status = count_in_range(test_array, array_size, 5, 12, &count);
    if (status != 0) {
        fprintf(stderr, "Error: count_in_range failed with status %d\n", status);
        return EXIT_FAILURE;
    }

    printf("Number of elements in range [5, 12]: %zu\n", count);

    status = count_in_range(NULL, array_size, 1, 10, &count);
    if (status != 0) {
        fprintf(stderr, "Error: NULL array check failed with status %d\n", status);
    }

    status = count_in_range(test_array, array_size, 10, 1, &count);
    if (status != 0) {
        fprintf(stderr, "Error: Invalid range check failed with status %d\n", status);
    }

    return EXIT_SUCCESS;
}