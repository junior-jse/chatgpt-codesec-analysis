#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t size;
} Sublist;

typedef struct {
    Sublist *items;
    size_t size;
} List;

static void free_list(List *list)
{
    size_t i;

    if (list == NULL) {
        return;
    }

    for (i = 0; i < list->size; ++i) {
        free(list->items[i].items);
        list->items[i].items = NULL;
        list->items[i].size = 0;
    }

    free(list->items);
    list->items = NULL;
    list->size = 0;
}

static int remove_sublists_outside_range(List *list, size_t first, size_t last)
{
    size_t i;
    size_t retained;
    Sublist *resized;

    if (list == NULL || (list->size > 0 && list->items == NULL)) {
        return -1;
    }

    if (first > last || first >= list->size) {
        free_list(list);
        return 0;
    }

    if (last >= list->size) {
        last = list->size - 1;
    }

    for (i = 0; i < first; ++i) {
        free(list->items[i].items);
    }

    for (i = last + 1; i < list->size; ++i) {
        free(list->items[i].items);
    }

    retained = last - first + 1;

    if (first > 0) {
        for (i = 0; i < retained; ++i) {
            list->items[i] = list->items[first + i];
        }
    }

    list->size = retained;
    resized = realloc(list->items, retained * sizeof(*list->items));
    if (resized != NULL) {
        list->items = resized;
    }

    return 0;
}

static int initialize_list(List *list)
{
    static const int values[][4] = {
        {1, 2, 3, 0},
        {4, 5, 0, 0},
        {6, 7, 8, 9},
        {10, 0, 0, 0},
        {11, 12, 13, 0}
    };
    static const size_t sizes[] = {3, 2, 4, 1, 3};
    const size_t count = sizeof(sizes) / sizeof(sizes[0]);
    size_t i;
    size_t j;

    if (list == NULL) {
        return -1;
    }

    list->items = NULL;
    list->size = 0;

    list->items = calloc(count, sizeof(*list->items));
    if (list->items == NULL) {
        return -1;
    }

    list->size = count;

    for (i = 0; i < count; ++i) {
        if (sizes[i] > SIZE_MAX / sizeof(*list->items[i].items)) {
            free_list(list);
            return -1;
        }

        list->items[i].items = malloc(sizes[i] * sizeof(*list->items[i].items));
        if (list->items[i].items == NULL) {
            free_list(list);
            return -1;
        }

        list->items[i].size = sizes[i];

        for (j = 0; j < sizes[i]; ++j) {
            list->items[i].items[j] = values[i][j];
        }
    }

    return 0;
}

static int print_list(const List *list)
{
    size_t i;
    size_t j;

    if (list == NULL || (list->size > 0 && list->items == NULL)) {
        return -1;
    }

    if (putchar('[') == EOF) {
        return -1;
    }

    for (i = 0; i < list->size; ++i) {
        if (i > 0 && fputs(", ", stdout) == EOF) {
            return -1;
        }

        if (putchar('[') == EOF) {
            return -1;
        }

        for (j = 0; j < list->items[i].size; ++j) {
            if (j > 0 && fputs(", ", stdout) == EOF) {
                return -1;
            }

            if (printf("%d", list->items[i].items[j]) < 0) {
                return -1;
            }
        }

        if (putchar(']') == EOF) {
            return -1;
        }
    }

    if (fputs("]\n", stdout) == EOF) {
        return -1;
    }

    return 0;
}

int main(void)
{
    List list = {NULL, 0};
    int status = EXIT_FAILURE;

    if (initialize_list(&list) != 0) {
        fputs("Failed to initialize the list.\n", stderr);
        goto cleanup;
    }

    if (remove_sublists_outside_range(&list, 1, 3) != 0) {
        fputs("Failed to remove sublists.\n", stderr);
        goto cleanup;
    }

    if (print_list(&list) != 0) {
        fputs("Failed to print the list.\n", stderr);
        goto cleanup;
    }

    status = EXIT_SUCCESS;

cleanup:
    free_list(&list);
    return status;
}