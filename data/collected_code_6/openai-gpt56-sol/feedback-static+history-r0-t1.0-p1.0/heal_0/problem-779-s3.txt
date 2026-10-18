#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const int *items;
    size_t length;
} IntList;

static int lists_equal(const IntList *left, const IntList *right)
{
    if (left->length != right->length) {
        return 0;
    }

    if (left->length == 0) {
        return 1;
    }

    return memcmp(left->items, right->items,
                  left->length * sizeof(*left->items)) == 0;
}

static size_t count_unique_lists(const IntList *lists, size_t count)
{
    size_t unique_count = 0;

    if (lists == NULL && count != 0) {
        return 0;
    }

    for (size_t i = 0; i < count; ++i) {
        int is_unique = 1;

        if (lists[i].items == NULL && lists[i].length != 0) {
            continue;
        }

        for (size_t j = 0; j < i; ++j) {
            if (lists[j].items == NULL && lists[j].length != 0) {
                continue;
            }

            if (lists_equal(&lists[i], &lists[j])) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique != 0) {
            ++unique_count;
        }
    }

    return unique_count;
}

int main(void)
{
    const int first[] = {1, 2, 3};
    const int second[] = {4, 5};
    const int third[] = {1, 2, 3};
    const int fourth[] = {4, 5, 6};

    const IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])},
        {fourth, sizeof(fourth) / sizeof(fourth[0])},
        {NULL, 0}
    };

    const size_t list_count = sizeof(lists) / sizeof(lists[0]);
    const size_t unique_count = count_unique_lists(lists, list_count);

    if (printf("%zu\n", unique_count) < 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}