#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    list.items = calloc(capacity, sizeof(Tuple));
    list.count = 0;
    list.capacity = capacity;
    return list;
}

void free_tuple_list(TupleList *list) {
    if (list->items != NULL) {
        for (int i = 0; i < list->count; i++) {
            if (list->items[i].elements != NULL) {
                free(list->items[i].elements);
            }
        }
        free(list->items);
        list->items = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

int remove_empty_tuples(TupleList *list) {
    if (list == NULL || list->items == NULL) {
        return -1;
    }

    int valid_count = 0;
    for (int i = 0; i < list->count; i++) {
        if (list->items[i].size > 0) {
            if (valid_count != i) {
                list->items[valid_count] = list->items[i];
                list->items[i].elements = NULL;
                list->items[i].size = 0;
            }
            valid_count++;
        } else {
            free(list->items[i].elements);
            list->items[i].elements = NULL;
        }
    }

    int removed = list->count - valid_count;
    list->count = valid_count;

    return removed;
}

void safe_memcpy(void *dest, size_t dest_size, const void *src, size_t src_size) {
    if (dest != NULL && src != NULL && dest_size >= src_size) {
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, src, src_size);
    }
}

int main() {
    int capacity = 5;
    TupleList list = create_tuple_list(capacity);
    if (list.items == NULL) {
        return EXIT_FAILURE;
    }

    int a[] = {1, 2};
    int b[] = {0};
    int d[] = {5, 6, 7};

    size_t a_size = sizeof(a);
    list.items[0].elements = malloc(a_size);
    if (list.items[0].elements != NULL) {
        safe_memcpy(list.items[0].elements, a_size, a, a_size);
        list.items[0].size = (int)(a_size / sizeof(int));
    }

    size_t b_size = sizeof(b);
    list.items[2].elements = malloc(b_size);
    if (list.items[2].elements != NULL) {
        safe_memcpy(list.items[2].elements, b_size, b, b_size);
        list.items[2].size = (int)(b_size / sizeof(int));
    }

    size_t d_size = sizeof(d);
    list.items[4].elements = malloc(d_size);
    if (list.items[4].elements != NULL) {
        safe_memcpy(list.items[4].elements, d_size, d, d_size);
        list.items[4].size = (int)(d_size / sizeof(int));
    }

    list.count = capacity;

    for (int i = 0; i < list.count; i++) {
        printf("Tuple %d: size=%d\n", i, list.items[i].size);
    }

    int removed = remove_empty_tuples(&list);
    printf("Removed: %d\n", removed);

    for (int i = 0; i < list.count; i++) {
        printf("Tuple %d: size=%d\n", i, list.items[i].size);
    }

    free_tuple_list(&list);

    return EXIT_SUCCESS;
}