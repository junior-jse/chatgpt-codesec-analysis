#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool contains(const int *list, size_t length, int value)
{
    for (size_t i = 0; i < length; ++i) {
        if (list[i] == value) {
            return true;
        }
    }

    return false;
}

static int list_difference(const int *first, size_t first_length,
                           const int *second, size_t second_length,
                           int **result, size_t *result_length)
{
    if (result == NULL || result_length == NULL ||
        (first == NULL && first_length != 0U) ||
        (second == NULL && second_length != 0U)) {
        return -1;
    }

    *result = NULL;
    *result_length = 0U;

    if (first_length == 0U) {
        return 0;
    }

    if (first_length > SIZE_MAX / sizeof(**result)) {
        return -1;
    }

    int *difference = malloc(first_length * sizeof(*difference));
    if (difference == NULL) {
        return -1;
    }

    size_t count = 0U;

    for (size_t i = 0U; i < first_length; ++i) {
        if (!contains(second, second_length, first[i]) &&
            !contains(difference, count, first[i])) {
            difference[count++] = first[i];
        }
    }

    if (count == 0U) {
        free(difference);
        return 0;
    }

    int *resized = realloc(difference, count * sizeof(*difference));
    if (resized != NULL) {
        difference = resized;
    }

    *result = difference;
    *result_length = count;
    return 0;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4, 5, 3};
    const int second[] = {2, 4, 6};
    int *difference = NULL;
    size_t difference_length = 0U;

    if (list_difference(first, sizeof(first) / sizeof(first[0]),
                        second, sizeof(second) / sizeof(second[0]),
                        &difference, &difference_length) != 0) {
        fputs("Failed to compute list difference.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < difference_length; ++i) {
        if (printf("%d%s", difference[i],
                   i + 1U < difference_length ? " " : "\n") < 0) {
            free(difference);
            return EXIT_FAILURE;
        }
    }

    if (difference_length == 0U && putchar('\n') == EOF) {
        free(difference);
        return EXIT_FAILURE;
    }

    free(difference);
    return EXIT_SUCCESS;
}