#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} List;

static int find_minimum_length_lists(const List *lists, size_t list_count,
                                     size_t **indices, size_t *result_count)
{
    size_t minimum_length;
    size_t count = 0;
    size_t *result;

    if (lists == NULL || list_count == 0 || indices == NULL ||
        result_count == NULL) {
        return 0;
    }

    *indices = NULL;
    *result_count = 0;
    minimum_length = lists[0].length;

    for (size_t i = 1; i < list_count; ++i) {
        if (lists[i].length < minimum_length) {
            minimum_length = lists[i].length;
        }
    }

    for (size_t i = 0; i < list_count; ++i) {
        if (lists[i].length == minimum_length) {
            ++count;
        }
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return 0;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return 0;
    }

    count = 0;
    for (size_t i = 0; i < list_count; ++i) {
        if (lists[i].length == minimum_length) {
            result[count++] = i;
        }
    }

    *indices = result;
    *result_count = count;
    return 1;
}

int main(void)
{
    int values0[] = {1, 2, 3};
    int values1[] = {4};
    int values2[] = {5, 6};
    int values3[] = {7};

    List lists[] = {
        {values0, sizeof(values0) / sizeof(values0[0])},
        {values1, sizeof(values1) / sizeof(values1[0])},
        {values2, sizeof(values2) / sizeof(values2[0])},
        {values3, sizeof(values3) / sizeof(values3[0])}
    };

    size_t *indices = NULL;
    size_t result_count = 0;
    size_t list_count = sizeof(lists) / sizeof(lists[0]);

    if (!find_minimum_length_lists(lists, list_count, &indices, &result_count)) {
        fputs("Unable to find minimum-length lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        printf("List %zu:", indices[i]);
        for (size_t j = 0; j < lists[indices[i]].length; ++j) {
            printf(" %d", lists[indices[i]].items[j]);
        }
        putchar('\n');
    }

    free(indices);
    return EXIT_SUCCESS;
}