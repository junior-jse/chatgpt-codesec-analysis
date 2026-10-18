#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    size_t length;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
    size_t capacity;
} TupleArray;

int init_tuple_array(TupleArray *arr, size_t initial_capacity) {
    if (arr == NULL || initial_capacity == 0) {
        return -1;
    }
    arr->tuples = malloc(initial_capacity * sizeof(Tuple));
    if (arr->tuples == NULL) {
        return -1;
    }
    arr->count = 0;
    arr->capacity = initial_capacity;
    return 0;
}

int add_tuple(TupleArray *arr, const int *data, size_t length) {
    if (arr == NULL || (data == NULL && length > 0)) {
        return -1;
    }
    if (arr->count >= arr->capacity) {
        size_t new_capacity = arr->capacity * 2;
        Tuple *new_tuples = realloc(arr->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return -1;
        }
        arr->tuples = new_tuples;
        arr->capacity = new_capacity;
    }
    arr->tuples[arr->count].data = NULL;
    if (length > 0) {
        arr->tuples[arr->count].data = malloc(length * sizeof(int));
        if (arr->tuples[arr->count].data == NULL) {
            return -1;
        }
        memcpy(arr->tuples[arr->count].data, data, length * sizeof(int));
    }
    arr->tuples[arr->count].length = length;
    arr->count++;
    return 0;
}

void remove_tuples_with_length(TupleArray *arr, size_t k) {
    if (arr == NULL || arr->tuples == NULL) {
        return;
    }
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < arr->count; read_idx++) {
        if (arr->tuples[read_idx].length == k) {
            free(arr->tuples[read_idx].data);
            arr->tuples[read_idx].data = NULL;
        } else {
            if (write_idx != read_idx) {
                arr->tuples[write_idx] = arr->tuples[read_idx];
            }
            write_idx++;
        }
    }
    arr->count = write_idx;
}

void free_tuple_array(TupleArray *arr) {
    if (arr == NULL) {
        return;
    }
    if (arr->tuples != NULL) {
        for (size_t i = 0; i < arr->count; i++) {
            free(arr->tuples[i].data);
            arr->tuples[i].data = NULL;
        }
        free(arr->tuples);
        arr->tuples = NULL;
    }
    arr->count = 0;
    arr->capacity = 0;
}

void print_tuple_array(const TupleArray *arr) {
    if (arr == NULL) {
        return;
    }
    for (size_t i = 0; i < arr->count; i++) {
        printf("(");
        for (size_t j = 0; j < arr->tuples[i].length; j++) {
            printf("%d", arr->tuples[i].data[j]);
            if (j < arr->tuples[i].length - 1) {
                printf(", ");
            }
        }
        printf(")");
        if (i < arr->count - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

int main(void) {
    TupleArray arr;
    if (init_tuple_array(&arr, 4) != 0) {
        fprintf(stderr, "Failed to initialize tuple array\n");
        return EXIT_FAILURE;
    }

    const int t1[] = {1, 2};
    const int t2[] = {3, 4, 5};
    const int t3[] = {6, 7};
    const int t4[] = {8};
    const int t5[] = {9, 10, 11};
    const int t6[] = {12, 13};

    if (add_tuple(&arr, t1, 2) != 0 ||
        add_tuple(&arr, t2, 3) != 0 ||
        add_tuple(&arr, t3, 2) != 0 ||
        add_tuple(&arr, t4, 1) != 0 ||
        add_tuple(&arr, t5, 3) != 0 ||
        add_tuple(&arr, t6, 2) != 0) {
        fprintf(stderr, "Failed to add tuples\n");
        free_tuple_array(&arr);
        return EXIT_FAILURE;
    }

    printf("Before removal: ");
    print_tuple_array(&arr);

    size_t k = 2;
    remove_tuples_with_length(&arr, k);

    printf("After removing tuples of length %zu: ", k);
    print_tuple_array(&arr);

    free_tuple_array(&arr);
    return EXIT_SUCCESS;
}