#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t size;
    size_t capacity;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
    size_t capacity;
} TupleList;

int init_tuple(Tuple *t, size_t capacity) {
    if (t == NULL || capacity == 0) {
        return -1;
    }
    t->data = malloc(capacity * sizeof(int));
    if (t->data == NULL) {
        return -1;
    }
    t->size = 0;
    t->capacity = capacity;
    return 0;
}

int init_tuple_list(TupleList *list, size_t capacity) {
    if (list == NULL || capacity == 0) {
        return -1;
    }
    list->tuples = malloc(capacity * sizeof(Tuple));
    if (list->tuples == NULL) {
        return -1;
    }
    list->count = 0;
    list->capacity = capacity;
    return 0;
}

int add_tuple(TupleList *list, const int *data, size_t size) {
    if (list == NULL || data == NULL || size == 0) {
        return -1;
    }
    if (list->count >= list->capacity) {
        return -1;
    }
    if (init_tuple(&list->tuples[list->count], size) != 0) {
        return -1;
    }
    memcpy(list->tuples[list->count].data, data, size * sizeof(int));
    list->tuples[list->count].size = size;
    list->count++;
    return 0;
}

int trim_tuple(Tuple *t, size_t k) {
    if (t == NULL || t->data == NULL) {
        return -1;
    }
    if (k == 0) {
        return 0;
    }
    if (k >= t->size) {
        t->size = 0;
        return 0;
    }
    t->size -= k;
    return 0;
}

int trim_tuple_list(TupleList *list, size_t k) {
    if (list == NULL) {
        return -1;
    }
    for (size_t i = 0; i < list->count; i++) {
        if (trim_tuple(&list->tuples[i], k) != 0) {
            return -1;
        }
    }
    return 0;
}

void free_tuple(Tuple *t) {
    if (t != NULL && t->data != NULL) {
        free(t->data);
        t->data = NULL;
        t->size = 0;
        t->capacity = 0;
    }
}

void free_tuple_list(TupleList *list) {
    if (list != NULL) {
        for (size_t i = 0; i < list->count; i++) {
            free_tuple(&list->tuples[i]);
        }
        free(list->tuples);
        list->tuples = NULL;
        list->count = 0;
        list->capacity = 0;
    }
}

void print_tuple_list(const TupleList *list) {
    if (list == NULL) {
        return;
    }
    for (size_t i = 0; i < list->count; i++) {
        printf("Tuple %zu: [", i);
        for (size_t j = 0; j < list->tuples[i].size; j++) {
            printf("%d", list->tuples[i].data[j]);
            if (j < list->tuples[i].size - 1) {
                printf(", ");
            }
        }
        printf("]\n");
    }
}

int main(void) {
    TupleList list;
    size_t k = 2;

    if (init_tuple_list(&list, 5) != 0) {
        fprintf(stderr, "Failed to initialize tuple list\n");
        return EXIT_FAILURE;
    }

    int data1[] = {1, 2, 3, 4, 5};
    int data2[] = {10, 20, 30};
    int data3[] = {7, 8, 9, 10, 11, 12};

    if (add_tuple(&list, data1, 5) != 0 ||
        add_tuple(&list, data2, 3) != 0 ||
        add_tuple(&list, data3, 6) != 0) {
        fprintf(stderr, "Failed to add tuples\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    printf("Before trimming:\n");
    print_tuple_list(&list);

    if (trim_tuple_list(&list, k) != 0) {
        fprintf(stderr, "Failed to trim tuples\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    printf("After trimming by %zu:\n", k);
    print_tuple_list(&list);

    free_tuple_list(&list);
    return EXIT_SUCCESS;
}