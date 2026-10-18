#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *elements;
    size_t size;
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
    size_t capacity;
} TupleList;

TupleList create_tuple_list(size_t capacity) {
    TupleList list;
    if (capacity == 0) {
        capacity = 4;
    }
    list.items = (Tuple *)malloc(capacity * sizeof(Tuple));
    if (list.items == NULL) {
        exit(EXIT_FAILURE);
    }
    list.count = 0;
    list.capacity = capacity;
    return list;
}

void add_tuple(TupleList *list, const int *elements, size_t size) {
    if (list->count >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity <= list->capacity) {
            new_capacity = list->capacity + 4;
        }
        Tuple *new_items = (Tuple *)realloc(list->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            exit(EXIT_FAILURE);
        }
        list->items = new_items;
        list->capacity = new_capacity;
    }
    list->items[list->count].size = size;
    list->items[list->count].elements = NULL;
    if (size > 0 && elements != NULL) {
        list->items[list->count].elements = (int *)malloc(size * sizeof(int));
        if (list->items[list->count].elements == NULL) {
            exit(EXIT_FAILURE);
        }
        memcpy(list->items[list->count].elements, elements, size * sizeof(int));
    }
    list->count++;
}

void remove_empty_tuples(TupleList *list) {
    size_t write_index = 0;
    for (size_t read_index = 0; read_index < list->count; read_index++) {
        if (list->items[read_index].size == 0) {
            free(list->items[read_index].elements);
            list->items[read_index].elements = NULL;
        } else {
            if (write_index != read_index) {
                list->items[write_index] = list->items[read_index];
                list->items[read_index].elements = NULL;
                list->items[read_index].size = 0;
            }
            write_index++;
        }
    }
    list->count = write_index;
}

void free_tuple_list(TupleList *list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->items[i].elements);
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int main() {
    TupleList list = create_tuple_list(4);

    const int a[] = {1, 2};
    const int c[] = {3, 4, 5};

    add_tuple(&list, a, 2);
    add_tuple(&list, NULL, 0);
    add_tuple(&list, c, 3);
    add_tuple(&list, NULL, 0);

    remove_empty_tuples(&list);

    for (size_t i = 0; i < list.count; i++) {
        printf("(");
        for (size_t j = 0; j < list.items[i].size; j++) {
            if (j > 0) {
                printf(", ");
            }
            printf("%d", list.items[i].elements[j]);
        }
        printf(")\n");
    }

    free_tuple_list(&list);

    return 0;
}