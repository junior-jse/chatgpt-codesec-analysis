#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    int *data;
    size_t size;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t size;
    size_t capacity;
} TupleList;

int init_tuple_list(TupleList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }
    if (initial_capacity > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }
    list->tuples = calloc(initial_capacity, sizeof(Tuple));
    if (list->tuples == NULL) {
        return -1;
    }
    list->size = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_tuple(TupleList *list, const int *data, size_t data_size) {
    if (list == NULL || list->tuples == NULL) {
        return -1;
    }
    if (data_size > 0 && data == NULL) {
        return -1;
    }
    if (data_size > SIZE_MAX / sizeof(int)) {
        return -1;
    }
    if (list->size >= list->capacity) {
        size_t new_capacity = list->capacity * 2;
        if (new_capacity > SIZE_MAX / sizeof(Tuple)) {
            return -1;
        }
        Tuple *new_tuples = realloc(list->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return -1;
        }
        for (size_t i = list->capacity; i < new_capacity; i++) {
            new_tuples[i].data = NULL;
            new_tuples[i].size = 0;
        }
        list->tuples = new_tuples;
        list->capacity = new_capacity;
    }

    if (data_size > 0) {
        int *new_data = malloc(data_size * sizeof(int));
        if (new_data == NULL) {
            return -1;
        }
        memcpy(new_data, data, data_size * sizeof(int));
        list->tuples[list->size].data = new_data;
    } else {
        list->tuples[list->size].data = NULL;
    }
    list->tuples[list->size].size = data_size;
    list->size++;
    return 0;
}

void remove_empty_tuples(TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }

    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < list->size; read_idx++) {
        if (list->tuples[read_idx].size > 0 && list->tuples[read_idx].data != NULL) {
            if (write_idx != read_idx) {
                list->tuples[write_idx] = list->tuples[read_idx];
                list->tuples[read_idx].data = NULL;
                list->tuples[read_idx].size = 0;
            }
            write_idx++;
        } else {
            free(list->tuples[read_idx].data);
            list->tuples[read_idx].data = NULL;
            list->tuples[read_idx].size = 0;
        }
    }
    list->size = write_idx;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }
    if (list->tuples != NULL) {
        for (size_t i = 0; i < list->size; i++) {
            free(list->tuples[i].data);
            list->tuples[i].data = NULL;
        }
        free(list->tuples);
        list->tuples = NULL;
    }
    list->size = 0;
    list->capacity = 0;
}

void print_tuple_list(const TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }
    for (size_t i = 0; i < list->size; i++) {
        printf("(");
        if (list->tuples[i].data != NULL) {
            for (size_t j = 0; j < list->tuples[i].size; j++) {
                printf("%d", list->tuples[i].data[j]);
                if (j < list->tuples[i].size - 1) {
                    printf(", ");
                }
            }
        }
        printf(")");
        if (i < list->size - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

int main(void) {
    TupleList list;
    int result = 0;

    if (init_tuple_list(&list, 4) != 0) {
        fprintf(stderr, "Failed to initialize tuple list\n");
        return EXIT_FAILURE;
    }

    const int data1[] = {1, 2, 3};
    const int data2[] = {4, 5};
    const int data3[] = {6};

    if (add_tuple(&list, data1, 3) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, NULL, 0) != 0) {
        fprintf(stderr, "Failed to add empty tuple\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, data2, 2) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, NULL, 0) != 0) {
        fprintf(stderr, "Failed to add empty tuple\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, data3, 1) != 0) {
        fprintf(stderr, "Failed to add tuple\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    printf("Before removal: ");
    print_tuple_list(&list);

    remove_empty_tuples(&list);

    printf("After removal: ");
    print_tuple_list(&list);

    free_tuple_list(&list);

    return result;
}