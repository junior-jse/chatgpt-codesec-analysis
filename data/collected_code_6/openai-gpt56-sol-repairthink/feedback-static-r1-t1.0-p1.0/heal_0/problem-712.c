#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *items;
    size_t length;
} IntList;

static bool lists_equal(const IntList *a, const IntList *b)
{
    if (a == NULL || b == NULL || a->length != b->length) {
        return false;
    }

    if (a->length > 0 && (a->items == NULL || b->items == NULL)) {
        return false;
    }

    for (size_t i = 0; i < a->length; ++i) {
        if (a->items[i] != b->items[i]) {
            return false;
        }
    }

    return true;
}

static size_t remove_duplicate_lists(IntList *lists, size_t count)
{
    if (lists == NULL) {
        return 0;
    }

    size_t unique_count = 0;

    for (size_t i = 0; i < count; ++i) {
        bool duplicate = false;

        for (size_t j = 0; j < unique_count; ++j) {
            if (lists_equal(&lists[i], &lists[j])) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            free(lists[i].items);
            lists[i].items = NULL;
            lists[i].length = 0;
        } else {
            if (i != unique_count) {
                lists[unique_count] = lists[i];
                lists[i].items = NULL;
                lists[i].length = 0;
            }
            ++unique_count;
        }
    }

    return unique_count;
}

static void free_lists(IntList *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(lists[i].items);
        lists[i].items = NULL;
        lists[i].length = 0;
    }

    free(lists);
}

static bool create_list(IntList *list, const int *values, size_t length)
{
    if (list == NULL || (length > 0 && values == NULL)) {
        return false;
    }

    list->items = NULL;
    list->length = 0;

    if (length == 0) {
        return true;
    }

    if (length > SIZE_MAX / sizeof(*list->items)) {
        return false;
    }

    list->items = malloc(length * sizeof(*list->items));
    if (list->items == NULL) {
        return false;
    }

    for (size_t i = 0; i < length; ++i) {
        list->items[i] = values[i];
    }

    list->length = length;
    return true;
}

static void print_lists(const IntList *lists, size_t count)
{
    if (lists == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        putchar('[');

        for (size_t j = 0; j < lists[i].length; ++j) {
            if (j > 0) {
                printf(", ");
            }
            printf("%d", lists[i].items[j]);
        }

        puts("]");
    }
}

int main(void)
{
    const int values[][3] = {
        {1, 2, 3},
        {4, 5, 0},
        {1, 2, 3},
        {6, 0, 0},
        {4, 5, 0}
    };
    const size_t lengths[] = {3, 2, 3, 1, 2};
    const size_t count = sizeof(lengths) / sizeof(lengths[0]);

    if (count > SIZE_MAX / sizeof(IntList)) {
        fputs("Allocation size overflow\n", stderr);
        return EXIT_FAILURE;
    }

    IntList *lists = calloc(count, sizeof(*lists));
    if (lists == NULL) {
        fputs("Memory allocation failed\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < count; ++i) {
        if (!create_list(&lists[i], values[i], lengths[i])) {
            fputs("Failed to create list\n", stderr);
            free_lists(lists, count);
            return EXIT_FAILURE;
        }
    }

    const size_t unique_count = remove_duplicate_lists(lists, count);
    print_lists(lists, unique_count);
    free_lists(lists, unique_count);

    return EXIT_SUCCESS;
}