#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int a;
    int b;
} Tuple;

typedef struct {
    Tuple *items;
    size_t count;
    size_t capacity;
} TupleArray;

int tuple_compare(Tuple t1, Tuple t2) {
    if (t1.a == t2.a && t1.b == t2.b) {
        return 1;
    }
    return 0;
}

int tuple_array_init(TupleArray *arr, size_t initial_capacity) {
    if (initial_capacity == 0) {
        initial_capacity = 1;
    }
    arr->items = (Tuple *)malloc(initial_capacity * sizeof(Tuple));
    if (arr->items == NULL) {
        return -1;
    }
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 0;
}

int tuple_array_push(TupleArray *arr, Tuple t) {
    if (arr->count == arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        Tuple *new_items = (Tuple *)realloc(arr->items, new_capacity * sizeof(Tuple));
        if (new_items == NULL) {
            return -1;
        }
        arr->items = new_items;
        arr->capacity = new_capacity;
    }
    arr->items[arr->count] = t;
    arr->count++;
    return 0;
}

void tuple_array_free(TupleArray *arr) {
    free(arr->items);
    arr->items = NULL;
    arr->count = 0;
    arr->capacity = 0;
}

int remove_matching_tuples(TupleArray *arr1, TupleArray *arr2, TupleArray *res1, TupleArray *res2) {
    if (tuple_array_init(res1, arr1->count) != 0) return -1;
    if (tuple_array_init(res2, arr2->count) != 0) {
        tuple_array_free(res1);
        return -1;
    }

    int *skip1 = (int *)calloc(arr1->count, sizeof(int));
    if (skip1 == NULL) {
        tuple_array_free(res1);
        tuple_array_free(res2);
        return -1;
    }
    int *skip2 = (int *)calloc(arr2->count, sizeof(int));
    if (skip2 == NULL) {
        free(skip1);
        tuple_array_free(res1);
        tuple_array_free(res2);
        return -1;
    }

    for (size_t i = 0; i < arr1->count; i++) {
        for (size_t j = 0; j < arr2->count; j++) {
            if (!skip2[j] && tuple_compare(arr1->items[i], arr2->items[j])) {
                skip1[i] = 1;
                skip2[j] = 1;
                break;
            }
        }
    }

    for (size_t i = 0; i < arr1->count; i++) {
        if (!skip1[i]) {
            if (tuple_array_push(res1, arr1->items[i]) != 0) {
                free(skip1);
                free(skip2);
                tuple_array_free(res1);
                tuple_array_free(res2);
                return -1;
            }
        }
    }

    for (size_t j = 0; j < arr2->count; j++) {
        if (!skip2[j]) {
            if (tuple_array_push(res2, arr2->items[j]) != 0) {
                free(skip1);
                free(skip2);
                tuple_array_free(res1);
                tuple_array_free(res2);
                return -1;
            }
        }
    }

    free(skip1);
    free(skip2);

    return 0;
}

int main() {
    TupleArray arr1, arr2, res1, res2;

    if (tuple_array_init(&arr1, 4) != 0) return 1;
    if (tuple_array_init(&arr2, 4) != 0) {
        tuple_array_free(&arr1);
        return 1;
    }

    tuple_array_push(&arr1, (Tuple){1, 2});
    tuple_array_push(&arr1, (Tuple){3, 4});
    tuple_array_push(&arr1, (Tuple){5, 6});

    tuple_array_push(&arr2, (Tuple){3, 4});
    tuple_array_push(&arr2, (Tuple){7, 8});

    if (remove_matching_tuples(&arr1, &arr2, &res1, &res2) == 0) {
        for (size_t i = 0; i < res1.count; i++) {
            printf("(%d, %d) ", res1.items[i].a, res1.items[i].b);
        }
        printf("\n");
        for (size_t i = 0; i < res2.count; i++) {
            printf("(%d, %d) ", res2.items[i].a, res2.items[i].b);
        }
        printf("\n");
        tuple_array_free(&res1);
        tuple_array_free(&res2);
    }

    tuple_array_free(&arr1);
    tuple_array_free(&arr2);

    return 0;
}