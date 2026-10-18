#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    int *elements;
    int size;
} Tuple;

typedef struct {
    Tuple *items;
    int count;
    int capacity;
} TupleList;

TupleList create_tuple_list(int capacity) {
    TupleList list;
    if (capacity <= 0 || (size_t)capacity > SIZE_MAX / sizeof(Tuple)) {
        exit(EXIT_FAILURE);
    }
    list.items = (Tuple *)malloc((size_t)capacity * sizeof(Tuple));
    if (list.items == NULL) {
        exit(EXIT_FAILURE);
    }
    list.count = 0;
    list.capacity = capacity;
    return list;
}

void add_tuple(TupleList *list, int *elements, int size) {
    if (list->count >= list->capacity) {
        if (list->capacity > INT_MAX / 2) {
            exit(EXIT_FAILURE);
        }
        list->capacity *= 2;
        size_t alloc_size_items = (size_t)list->capacity * sizeof(Tuple);
        if (alloc_size_items / sizeof(Tuple) != (size_t)list->capacity) {
            exit(EXIT_FAILURE);
        }
        Tuple *new_items = (Tuple *)realloc(list->items, alloc_size_items);
        if (new_items == NULL) {
            exit(EXIT_FAILURE);
        }
        list->items = new_items;
    }
    list->items[list->count].size = size;
    list->items[list->count].elements = NULL;
    if (size > 0) {
        size_t alloc_size_elements = (size_t)size * sizeof(int);
        if (alloc_size_elements / sizeof(int) != (size_t)size) {
            exit(EXIT_FAILURE);
        }
        list->items[list->count].elements = (int *)malloc(alloc_size_elements);
        if (list->items[list->count].elements == NULL) {
            exit(EXIT_FAILURE);
        }
        memcpy(list->items[list->count].elements, elements, alloc_size_elements);
    }
    list->count++;
}

void remove_empty_tuples(TupleList *list) {
    int write_index = 0;
    for (int read_index = 0; read_index < list->count; read_index++) {
        if (list->items[read_index].size == 0) {
            free(list->items[read_index].elements);
        } else {
            if (write_index != read_index) {
                list->items[write_index] = list->items[read_index];
            }
            write_index++;
        }
    }
    list->count = write_index;
}

void free_tuple_list(TupleList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->items[i].elements);
    }
    free(list->items);
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int main() {
    TupleList list = create_tuple_list(4);

    int a[] = {1, 2};
    int b[] = {};
    int c[] = {3, 4, 5};
    int d[] = {};

    add_tuple(&list, a, 2);
    add_tuple(&list, b, 0);
    add_tuple(&list, c, 3);
    add_tuple(&list, d, 0);

    remove_empty_tuples(&list);

    for (int i = 0; i < list.count; i++) {
        printf("(");
        for (int j = 0; j < list.items[i].size; j++) {
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