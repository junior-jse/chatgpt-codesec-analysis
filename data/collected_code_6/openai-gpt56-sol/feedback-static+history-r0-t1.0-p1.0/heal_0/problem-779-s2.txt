#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    const int *values;
    size_t length;
} IntList;

static bool lists_equal(const IntList *first, const IntList *second)
{
    if (first == NULL || second == NULL || first->length != second->length) {
        return false;
    }

    if (first->length > 0 && (first->values == NULL || second->values == NULL)) {
        return false;
    }

    for (size_t i = 0; i < first->length; ++i) {
        if (first->values[i] != second->values[i]) {
            return false;
        }
    }

    return true;
}

static bool count_unique_lists(const IntList *lists, size_t count, size_t *result)
{
    if (result == NULL || (count > 0 && lists == NULL)) {
        return false;
    }

    *result = 0;

    for (size_t i = 0; i < count; ++i) {
        if (lists[i].length > 0 && lists[i].values == NULL) {
            return false;
        }

        bool unique = true;

        for (size_t j = 0; j < i; ++j) {
            if (lists_equal(&lists[i], &lists[j])) {
                unique = false;
                break;
            }
        }

        if (unique) {
            ++(*result);
        }
    }

    return true;
}

int main(void)
{
    const int values1[] = {1, 2, 3};
    const int values2[] = {4, 5};
    const int values3[] = {1, 2, 3};
    const int values4[] = {6};

    const IntList lists[] = {
        {values1, sizeof values1 / sizeof values1[0]},
        {values2, sizeof values2 / sizeof values2[0]},
        {values3, sizeof values3 / sizeof values3[0]},
        {values4, sizeof values4 / sizeof values4[0]},
        {NULL, 0}
    };

    size_t unique_count = 0;

    if (!count_unique_lists(
            lists,
            sizeof lists / sizeof lists[0],
            &unique_count)) {
        fputs("Failed to count unique lists.\n", stderr);
        return EXIT_FAILURE;
    }

    if (printf("%zu\n", unique_count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}