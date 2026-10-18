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

    if (list == NULL) {
        return 0;
    }

    if (list->size == list->capacity) {
        if (list->capacity == 0) {
            new_capacity = 4;
        } else {
            if (list->capacity > SIZE_MAX / 2) {
                return 0;
            }
            new_capacity = list->capacity * 2;
        }

        if (new_capacity > SIZE_MAX / sizeof(*list->items)) {
            return 0;
        }

        new_items = realloc(list->items, new_capacity * sizeof(*list->items));
        if (new_items == NULL) {
            return 0;
        }

        list->items = new_items;
        list->capacity = new_capacity;
    }

    list->items[list->size] = tuple;
    list->size++;
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
        fputs("Failed to initialize list.\n", stderr);
        return EXIT_FAILURE;
    }

    if (!tuple_list_add(&list, tuple)) {
        fputs("Failed to add tuple.\n", stderr);
        tuple_list_destroy(&list);
        return EXIT_FAILURE;
    }

    printf("(%d, %d)\n", list.items[0].first, list.items[0].second);

    tuple_list_destroy(&list);
    return EXIT_SUCCESS;
}