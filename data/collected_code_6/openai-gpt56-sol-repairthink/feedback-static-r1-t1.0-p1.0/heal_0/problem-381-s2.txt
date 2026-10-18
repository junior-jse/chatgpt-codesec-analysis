#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    int *values;
    size_t length;
} IntList;

static int compare_by_index(const IntList *left, const IntList *right, size_t index)
{
    if (left->values[index] < right->values[index]) {
        return -1;
    }
    if (left->values[index] > right->values[index]) {
        return 1;
    }
    return 0;
}

static int sort_lists_by_index(IntList *lists, size_t count, size_t index)
{
    size_t i;

    if (lists == NULL && count != 0U) {
        return -1;
    }

    for (i = 0U; i < count; ++i) {
        if (lists[i].values == NULL || index >= lists[i].length) {
            return -1;
        }
    }

    for (i = 1U; i < count; ++i) {
        IntList current = lists[i];
        size_t j = i;

        while (j > 0U && compare_by_index(&current, &lists[j - 1U], index) < 0) {
            lists[j] = lists[j - 1U];
            --j;
        }
        lists[j] = current;
    }

    return 0;
}

static void print_lists(const IntList *lists, size_t count)
{
    size_t i;

    for (i = 0U; i < count; ++i) {
        size_t j;

        for (j = 0U; j < lists[i].length; ++j) {
            printf("%d%s", lists[i].values[j],
                   j + 1U == lists[i].length ? "" : " ");
        }
        putchar('\n');
    }
}

int main(void)
{
    int row0[] = {4, 8, 2};
    int row1[] = {1, 3, 9};
    int row2[] = {7, 5, 6};
    int row3[] = {2, 1, 4};
    IntList lists[] = {
        {row0, sizeof(row0) / sizeof(row0[0])},
        {row1, sizeof(row1) / sizeof(row1[0])},
        {row2, sizeof(row2) / sizeof(row2[0])},
        {row3, sizeof(row3) / sizeof(row3[0])}
    };
    const size_t count = sizeof(lists) / sizeof(lists[0]);
    const size_t index = 1U;

    if (sort_lists_by_index(lists, count, index) != 0) {
        fputs("Unable to sort lists\n", stderr);
        return EXIT_FAILURE;
    }

    print_lists(lists, count);
    return EXIT_SUCCESS;
}