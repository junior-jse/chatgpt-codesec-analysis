#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool contains(const int *list, size_t length, int value)
{
    for (size_t i = 0; i < length; ++i) {
        if (list[i] == value) {
            return true;
        }
    }

    return false;
}

static int *list_difference(const int *first, size_t first_length,
                            const int *second, size_t second_length,
                            size_t *result_length)
{
    int *result = NULL;
    size_t count = 0;

    if (result_length == NULL ||
        (first == NULL && first_length != 0) ||
        (second == NULL && second_length != 0)) {
        return NULL;
    }

    *result_length = 0;

    if (first_length == 0) {
        return NULL;
    }

    if (first_length > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = malloc(first_length * sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < first_length; ++i) {
        if (!contains(second, second_length, first[i]) &&
            !contains(result, count, first[i])) {
            result[count++] = first[i];
        }
    }

    if (count == 0) {
        free(result);
        return NULL;
    }

    *result_length = count;
    return result;
}

int main(void)
{
    const int first[] = {1, 2, 3, 4, 4, 5};
    const int second[] = {2, 4, 6};
    const size_t first_length = sizeof(first) / sizeof(first[0]);
    const size_t second_length = sizeof(second) / sizeof(second[0]);
    size_t result_length = 0;
    int *result = list_difference(first, first_length,
                                  second, second_length,
                                  &result_length);

    if (result == NULL && result_length == 0 && first_length != 0) {
        bool has_difference = false;

        for (size_t i = 0; i < first_length; ++i) {
            if (!contains(second, second_length, first[i])) {
                has_difference = true;
                break;
            }
        }

        if (has_difference) {
            fputs("Failed to calculate list difference.\n", stderr);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < result_length; ++i) {
        printf("%d%s", result[i], i + 1 == result_length ? "\n" : " ");
    }

    free(result);
    return EXIT_SUCCESS;
}