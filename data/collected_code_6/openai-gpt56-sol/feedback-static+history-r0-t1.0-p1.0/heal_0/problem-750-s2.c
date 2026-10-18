#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int first;
    int second;
} Tuple;

typedef struct {
    Tuple *items;
    size_t size;
    size_t capacity;
} TupleList;

static int tuple_list_init(TupleList *list)
{
    if (list == NULL) {
        return 0;
    }

    list->items = NULL;
    list->size = 0;
    list->capacity = 0;
    return 1;
}

static int tuple_list_add(TupleList *list, Tuple tuple)
{
    Tuple *new_items;
    size_t new_capacity;

    if (list == NULL || list->size == SIZE_MAX) {
        return 0;
    }

    if (list->size == list->capacity) {
        new_capacity = list->capacity == 0 ? 4 : list->capacity * 2;

        if (new_capacity < list->capacity ||
            new_capacity > SIZE_MAX / sizeof(*list->items)) {
            return 0;
        }

        new_items = realloc(list->items, new_capacity * sizeof(*list->items));
        if (new_items == NULL) {
            return 0;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    list->items[list->size++] = tuple;
    return 1;
}

static void tuple_list_destroy(TupleList *list)
{
    if (list != NULL) {
        free(list->items);
        list->items = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

int main(void)
{
    TupleList list;
    Tuple tuple = {10, 20};

    if (!tuple_list_init(&list)) {
        fputs("Failed to initialize the list.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!tuple_list_add(&list, tuple)) {
        fputs("Failed to add the tuple.\n", stderr);
        tuple_list_destroy(&list);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < list.size; ++i) {
        if (printf("(%d, %d)\n", list.items[i].first, list.items[i].second) < 0) {
            fputs("Failed to write output.\n", stderr);
            tuple_list_destroy(&list);
            return EXIT_FAILURE;
        }
    }

    tuple_list_destroy(&list);
    return EXIT_SUCCESS;
}