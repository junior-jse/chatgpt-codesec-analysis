#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static bool lists_equal(const IntList *first, const IntList *second)
{
    if (first == NULL || second == NULL || first->length != second->length) {
        return false;
    }

    if (first->length == 0U) {
        return true;
    }

    if (first->items == NULL || second->items == NULL) {
        return false;
    }

    for (size_t i = 0U; i < first->length; ++i) {
        if (first->items[i] != second->items[i]) {
            return false;
        }
    }

    return true;
}

static size_t count_unique_lists(const IntList *lists, size_t count)
{
    size_t unique_count = 0U;

    if (lists == NULL && count != 0U) {
        return 0U;
    }

    for (size_t i = 0U; i < count; ++i) {
        bool is_unique = true;

        for (size_t j = 0U; j < i; ++j) {
            if (lists_equal(&lists[i], &lists[j])) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            ++unique_count;
        }
    }

    return unique_count;
}

int main(void)
{
    int first[] = {1, 2, 3};
    int second[] = {4, 5};
    int third[] = {1, 2, 3};
    int fourth[] = {6};

    const IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])},
        {fourth, sizeof(fourth) / sizeof(fourth[0])}
    };

    const size_t count = sizeof(lists) / sizeof(lists[0]);
    const size_t unique_count = count_unique_lists(lists, count);

    if (printf("%zu\n", unique_count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}