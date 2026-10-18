#include <stdio.h>
#include <stdlib.h>

static int contains(const int *values, size_t count, int value)
{
    for (size_t i = 0; i < count; ++i) {
        if (values[i] == value) {
            return 1;
        }
    }

    return 0;
}

static int list_difference(const int *first, size_t first_count,
                           const int *second, size_t second_count,
                           int **result, size_t *result_count)
{
    int *difference = NULL;
    size_t count = 0;

    if (result == NULL || result_count == NULL ||
        (first_count > 0 && first == NULL) ||
        (second_count > 0 && second == NULL)) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (first_count == 0) {
        return 0;
    }

    if (first_count > SIZE_MAX / sizeof(*difference)) {
        return -1;
    }

    difference = malloc(first_count * sizeof(*difference));
    if (difference == NULL) {
        return -1;
    }

    for (size_t i = 0; i < first_count; ++i) {
        if (!contains(second, second_count, first[i])) {
            difference[count++] = first[i];
        }
    }

    if (count == 0) {
        free(difference);
        return 0;
    }

    *result = difference;
    *result_count = count;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4, 5};
    const int second[] = {2, 4, 6};
    int *difference = NULL;
    size_t difference_count = 0;

    if (list_difference(first, sizeof(first) / sizeof(first[0]),
                        second, sizeof(second) / sizeof(second[0]),
                        &difference, &difference_count) != 0) {
        fputs("Unable to compute list difference.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < difference_count; ++i) {
        if (printf("%d%s", difference[i],
                   i + 1 < difference_count ? " " : "\n") < 0) {
            free(difference);
            return EXIT_FAILURE;
        }
    }

    if (difference_count == 0 && putchar('\n') == EOF) {
        free(difference);
        return EXIT_FAILURE;
    }

    free(difference);
    return EXIT_SUCCESS;
}