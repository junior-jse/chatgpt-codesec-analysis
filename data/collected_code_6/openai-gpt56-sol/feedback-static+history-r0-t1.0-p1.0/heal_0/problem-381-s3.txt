#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *values;
    size_t length;
} IntList;

static size_t sort_index;

static int compare_lists(const void *lhs, const void *rhs)
{
    const IntList *a = lhs;
    const IntList *b = rhs;

    if (a->values[sort_index] < b->values[sort_index]) {
        return -1;
    }
    if (a->values[sort_index] > b->values[sort_index]) {
        return 1;
    }
    return 0;
}

static int sort_lists_by_index(IntList *lists, size_t count, size_t index)
{
    size_t i;

    if (count == 0) {
        return 0;
    }
    if (lists == NULL) {
        errno = EINVAL;
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (lists[i].values == NULL || index >= lists[i].length) {
            errno = EINVAL;
            return -1;
        }
    }

    sort_index = index;
    qsort(lists, count, sizeof(*lists), compare_lists);
    return 0;
}

static void print_lists(const IntList *lists, size_t count)
{
    size_t i;
    size_t j;

    for (i = 0; i < count; ++i) {
        for (j = 0; j < lists[i].length; ++j) {
            if (j > 0) {
                putchar(' ');
            }
            printf("%d", lists[i].values[j]);
        }
        putchar('\n');
    }
}

int main(void)
{
    int first[] = {3, 9, 1};
    int second[] = {2, 4, 8};
    int third[] = {7, 1, 5};
    IntList lists[] = {
        {first, sizeof(first) / sizeof(first[0])},
        {second, sizeof(second) / sizeof(second[0])},
        {third, sizeof(third) / sizeof(third[0])}
    };
    size_t count = sizeof(lists) / sizeof(lists[0]);

    if (sort_lists_by_index(lists, count, 1) != 0) {
        perror("sort_lists_by_index");
        return EXIT_FAILURE;
    }

    print_lists(lists, count);
    return EXIT_SUCCESS;
}