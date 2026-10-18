#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t length;
} List;

static int find_minimum_lists(const List *lists, size_t count,
                              const List ***result, size_t *result_count)
{
    const List **matches;
    size_t minimum;
    size_t matches_count = 0;
    size_t i;

    if (result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (lists == NULL || count == 0) {
        return 0;
    }

    if (count > SIZE_MAX / sizeof(*matches)) {
        return -1;
    }

    minimum = lists[0].length;

    for (i = 1; i < count; ++i) {
        if (lists[i].length < minimum) {
            minimum = lists[i].length;
        }
    }

    matches = malloc(count * sizeof(*matches));
    if (matches == NULL) {
        return -1;
    }

    for (i = 0; i < count; ++i) {
        if (lists[i].length == minimum) {
            matches[matches_count++] = &lists[i];
        }
    }

    *result = matches;
    *result_count = matches_count;
    return 0;
}

int main(void)
{
    int values1[] = {1, 2, 3};
    int values2[] = {4};
    int values3[] = {5, 6};
    int values4[] = {7};

    List lists[] = {
        {values1, sizeof(values1) / sizeof(values1[0])},
        {values2, sizeof(values2) / sizeof(values2[0])},
        {values3, sizeof(values3) / sizeof(values3[0])},
        {values4, sizeof(values4) / sizeof(values4[0])}
    };

    const List **minimum_lists = NULL;
    size_t minimum_count = 0;
    size_t list_count = sizeof(lists) / sizeof(lists[0]);
    size_t i;
    size_t j;

    if (find_minimum_lists(lists, list_count,
                           &minimum_lists, &minimum_count) != 0) {
        fputs("Failed to find minimum-length lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < minimum_count; ++i) {
        putchar('[');
        for (j = 0; j < minimum_lists[i]->length; ++j) {
            if (j > 0) {
                fputs(", ", stdout);
            }
            printf("%d", minimum_lists[i]->items[j]);
        }
        puts("]");
    }

    free(minimum_lists);
    return EXIT_SUCCESS;
}