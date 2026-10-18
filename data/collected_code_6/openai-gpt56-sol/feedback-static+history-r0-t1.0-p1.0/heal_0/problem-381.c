#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static int compare_by_index(const IntList *left, const IntList *right, size_t index)
{
    if (left->items[index] < right->items[index]) {
        return -1;
    }
    if (left->items[index] > right->items[index]) {
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
        if (lists[i].items == NULL || index >= lists[i].length) {
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
            printf("%d%s", lists[i].items[j],
                   j + 1U == lists[i].length ? "" : " ");
        }
        putchar('\n');
    }
}

int main(void)
{
    int values[][3] = {
        {1, 30, 4},
        {2, 10, 5},
        {3, 20, 6}
    };
    const size_t count = sizeof(values) / sizeof(values[0]);
    IntList *lists;
    size_t i;
    int result;

    lists = calloc(count, sizeof(*lists));
    if (lists == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < count; ++i) {
        lists[i].items = values[i];
        lists[i].length = sizeof(values[i]) / sizeof(values[i][0]);
    }

    result = sort_lists_by_index(lists, count, 1U);
    if (result != 0) {
        fputs("Invalid list or sort index\n", stderr);
        free(lists);
        return EXIT_FAILURE;
    }

    print_lists(lists, count);
    free(lists);

    return EXIT_SUCCESS;
}