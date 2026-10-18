#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *items;
    size_t size;
    size_t capacity;
} TupleList;

int list_init(TupleList *list, size_t initial_capacity) {
    list->items = (Tuple *)malloc(initial_capacity * sizeof(Tuple));
    if (list->items == NULL) {
        return -1;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return 0;
}

int list_add(TupleList *list, Tuple tuple) {
    if (list == NULL || list->items == NULL) {
        return -1;
    }
    if (list->size == list->capacity) {
        size_t new_capacity = list->capacity == 0 ? 1 : list->capacity * 2;
        Tuple *new_items = (Tuple *)realloc(list->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            return -1;
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->size] = tuple;
    list->size++;
    return 0;
}

void list_free(TupleList *list) {
    if (list != NULL) {
        free(list->items);
        list->items = NULL;
        list->size = 0;
        list->capacity = 0;
    }
}

int main(void) {
    TupleList list;
    if (list_init(&list, 2) != 0) {
        return 1;
    }
    Tuple t1 = {1, 2};
    Tuple t2 = {3, 4};
    Tuple t3 = {5, 6};
    if (list_add(&list, t1) != 0) {
        list_free(&list);
        return 1;
    }
    if (list_add(&list, t2) != 0) {
        list_free(&list);
        return 1;
    }
    if (list_add(&list, t3) != 0) {
        list_free(&list);
        return 1;
    }
    list_free(&list);
    return 0;
}