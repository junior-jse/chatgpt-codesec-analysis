#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    int *items;
    size_t size;
} IntList;

typedef struct {
    IntList *lists;
    size_t size;
} ListOfLists;

static bool lists_equal(const IntList *a, const IntList *b)
{
    if (a == NULL || b == NULL || a->size != b->size) {
        return false;
    }

    for (size_t i = 0; i < a->size; ++i) {
        if (a->items[i] != b->items[i]) {
            return false;
        }
    }

    return true;
}

static void free_int_list(IntList *list)
{
    if (list == NULL) {
        return;
    }

    free(list->items);
    list->items = NULL;
    list->size = 0;
}

static void free_list_of_lists(ListOfLists *collection)
{
    if (collection == NULL) {
        return;
    }

    for (size_t i = 0; i < collection->size; ++i) {
        free_int_list(&collection->lists[i]);
    }

    free(collection->lists);
    collection->lists = NULL;
    collection->size = 0;
}

static void remove_duplicate_lists(ListOfLists *collection)
{
    if (collection == NULL || collection->lists == NULL) {
        return;
    }

    size_t unique_count = 0;

    for (size_t i = 0; i < collection->size; ++i) {
        bool duplicate = false;

        for (size_t j = 0; j < unique_count; ++j) {
            if (lists_equal(&collection->lists[i], &collection->lists[j])) {
                duplicate = true;
                break;
            }
        }

        if (duplicate) {
            free_int_list(&collection->lists[i]);
        } else {
            if (i != unique_count) {
                collection->lists[unique_count] = collection->lists[i];
                collection->lists[i].items = NULL;
                collection->lists[i].size = 0;
            }
            ++unique_count;
        }
    }

    collection->size = unique_count;

    if (unique_count == 0) {
        free(collection->lists);
        collection->lists = NULL;
        return;
    }

    IntList *resized = realloc(
        collection->lists,
        unique_count * sizeof(*collection->lists)
    );

    if (resized != NULL) {
        collection->lists = resized;
    }
}

static bool create_list(IntList *list, const int *values, size_t count)
{
    if (list == NULL || (values == NULL && count != 0)) {
        return false;
    }

    list->items = NULL;
    list->size = 0;

    if (count == 0) {
        return true;
    }

    if (count > SIZE_MAX / sizeof(*list->items)) {
        return false;
    }

    list->items = malloc(count * sizeof(*list->items));
    if (list->items == NULL) {
        return false;
    }

    for (size_t i = 0; i < count; ++i) {
        list->items[i] = values[i];
    }

    list->size = count;
    return true;
}

static void print_list_of_lists(const ListOfLists *collection)
{
    if (collection == NULL) {
        return;
    }

    putchar('[');

    for (size_t i = 0; i < collection->size; ++i) {
        if (i != 0) {
            printf(", ");
        }

        putchar('[');

        for (size_t j = 0; j < collection->lists[i].size; ++j) {
            if (j != 0) {
                printf(", ");
            }

            printf("%d", collection->lists[i].items[j]);
        }

        putchar(']');
    }

    puts("]");
}

int main(void)
{
    const int values[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {1, 2, 3},
        {7, 8, 9},
        {4, 5, 6}
    };
    const size_t list_count = sizeof(values) / sizeof(values[0]);
    ListOfLists collection = {NULL, 0};

    if (list_count > SIZE_MAX / sizeof(*collection.lists)) {
        fputs("Input is too large.\n", stderr);
        return EXIT_FAILURE;
    }

    collection.lists = calloc(list_count, sizeof(*collection.lists));
    if (collection.lists == NULL) {
        fputs("Memory allocation failed.\n", stderr);
        return EXIT_FAILURE;
    }

    collection.size = list_count;

    for (size_t i = 0; i < list_count; ++i) {
        if (!create_list(
                &collection.lists[i],
                values[i],
                sizeof(values[i]) / sizeof(values[i][0]))) {
            fputs("Memory allocation failed.\n", stderr);
            free_list_of_lists(&collection);
            return EXIT_FAILURE;
        }
    }

    remove_duplicate_lists(&collection);
    print_list_of_lists(&collection);
    free_list_of_lists(&collection);

    return EXIT_SUCCESS;
}