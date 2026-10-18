#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static int find_max_length_lists(const IntList *lists,
                                 size_t list_count,
                                 size_t **indices,
                                 size_t *index_count)
{
    size_t max_length;
    size_t count = 0U;
    size_t *result;
    size_t i;

    if (lists == NULL || list_count == 0U ||
        indices == NULL || index_count == NULL) {
        return -1;
    }

    *indices = NULL;
    *index_count = 0U;
    max_length = lists[0].length;

    for (i = 1U; i < list_count; ++i) {
        if (lists[i].length > max_length) {
            max_length = lists[i].length;
        }
    }

    for (i = 0U; i < list_count; ++i) {
        if (lists[i].length == max_length) {
            ++count;
        }
    }

    if (count > SIZE_MAX / sizeof(*result)) {
        return -1;
    }

    result = malloc(count * sizeof(*result));
    if (result == NULL) {
        return -1;
    }

    count = 0U;
    for (i = 0U; i < list_count; ++i) {
        if (lists[i].length == max_length) {
            result[count++] = i;
        }
    }

    *indices = result;
    *index_count = count;
    return 0;
}

int main(void)
{
    int values0[] = {1, 2};
    int values1[] = {3, 4, 5, 6};
    int values2[] = {7};
    int values3[] = {8, 9, 10, 11};
    IntList lists[] = {
        {values0, sizeof(values0) / sizeof(values0[0])},
        {values1, sizeof(values1) / sizeof(values1[0])},
        {values2, sizeof(values2) / sizeof(values2[0])},
        {values3, sizeof(values3) / sizeof(values3[0])}
    };
    const size_t list_count = sizeof(lists) / sizeof(lists[0]);
    size_t *indices = NULL;
    size_t index_count = 0U;
    size_t i;
    size_t j;

    if (find_max_length_lists(lists, list_count, &indices, &index_count) != 0) {
        fputs("Failed to find maximum-length lists.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0U; i < index_count; ++i) {
        const size_t index = indices[i];

        printf("List %zu:", index);
        for (j = 0U; j < lists[index].length; ++j) {
            printf(" %d", lists[index].items[j]);
        }
        putchar('\n');
    }

    free(indices);
    return EXIT_SUCCESS;
}