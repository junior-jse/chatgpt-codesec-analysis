#include <stdio.h>
#include <stdlib.h>

typedef struct List {
    int *items;
    size_t length;
} List;

static void free_lists(List *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(lists[i].items);
    }

    free(lists);
}

static int remove_empty_lists(List **lists, size_t *count)
{
    if (lists == NULL || count == NULL || (*count > 0U && *lists == NULL)) {
        return -1;
    }

    size_t write_index = 0U;

    for (size_t read_index = 0U; read_index < *count; ++read_index) {
        if ((*lists)[read_index].length == 0U) {
            free((*lists)[read_index].items);
            (*lists)[read_index].items = NULL;
        } else {
            if ((*lists)[read_index].items == NULL) {
                return -1;
            }

            if (write_index != read_index) {
                (*lists)[write_index] = (*lists)[read_index];
                (*lists)[read_index].items = NULL;
                (*lists)[read_index].length = 0U;
            }

            ++write_index;
        }
    }

    if (write_index == 0U) {
        free(*lists);
        *lists = NULL;
    } else if (write_index < *count) {
        List *resized = realloc(*lists, write_index * sizeof(**lists));
        if (resized != NULL) {
            *lists = resized;
        }
    }

    *count = write_index;
    return 0;
}

int main(void)
{
    const size_t initial_count = 5U;
    List *lists = calloc(initial_count, sizeof(*lists));

    if (lists == NULL) {
        fprintf(stderr, "Allocation failed\n");
        return EXIT_FAILURE;
    }

    lists[0].length = 3U;
    lists[0].items = malloc(lists[0].length * sizeof(*lists[0].items));
    lists[2].length = 2U;
    lists[2].items = malloc(lists[2].length * sizeof(*lists[2].items));
    lists[4].length = 1U;
    lists[4].items = malloc(lists[4].length * sizeof(*lists[4].items));

    if (lists[0].items == NULL || lists[2].items == NULL ||
        lists[4].items == NULL) {
        fprintf(stderr, "Allocation failed\n");
        free_lists(lists, initial_count);
        return EXIT_FAILURE;
    }

    lists[0].items[0] = 1;
    lists[0].items[1] = 2;
    lists[0].items[2] = 3;
    lists[2].items[0] = 4;
    lists[2].items[1] = 5;
    lists[4].items[0] = 6;

    size_t count = initial_count;

    if (remove_empty_lists(&lists, &count) != 0) {
        fprintf(stderr, "Invalid list data\n");
        free_lists(lists, count);
        return EXIT_FAILURE;
    }

    for (size_t i = 0U; i < count; ++i) {
        for (size_t j = 0U; j < lists[i].length; ++j) {
            printf("%d%s", lists[i].items[j],
                   j + 1U == lists[i].length ? "\n" : " ");
        }
    }

    free_lists(lists, count);
    return EXIT_SUCCESS;
}