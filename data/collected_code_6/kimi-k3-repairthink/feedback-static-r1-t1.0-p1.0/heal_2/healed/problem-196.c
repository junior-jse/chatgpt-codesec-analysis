#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char *data;
    size_t length;
} Tuple;

typedef struct {
    Tuple *tuples;
    size_t count;
    size_t capacity;
} TupleList;

int init_tuple_list(TupleList *list, size_t initial_capacity) {
    if (list == NULL || initial_capacity == 0) {
        return -1;
    }

    if (initial_capacity > SIZE_MAX / sizeof(Tuple)) {
        return -1;
    }

    list->tuples = malloc(initial_capacity * sizeof(Tuple));
    if (list->tuples == NULL) {
        return -1;
    }

    list->count = 0;
    list->capacity = initial_capacity;
    return 0;
}

int add_tuple(TupleList *list, const char *data) {
    if (list == NULL || data == NULL) {
        return -1;
    }

    if (list->count >= list->capacity) {
        if (list->capacity > SIZE_MAX / 2 / sizeof(Tuple)) {
            return -1;
        }
        size_t new_capacity = list->capacity * 2;
        Tuple *new_tuples = realloc(list->tuples, new_capacity * sizeof(Tuple));
        if (new_tuples == NULL) {
            return -1;
        }
        list->tuples = new_tuples;
        list->capacity = new_capacity;
    }

    size_t len = strnlen(data, MAX_STRING_LENGTH);
    if (len >= MAX_STRING_LENGTH) {
        return -1;
    }

    size_t dest_size = len + 1;

    char *dest = malloc(dest_size);
    if (dest == NULL) {
        return -1;
    }

    int written = snprintf(dest, dest_size, "%s", data);
    if (written < 0 || (size_t)written >= dest_size) {
        free(dest);
        return -1;
    }

    list->tuples[list->count].data = dest;
    list->tuples[list->count].length = len;
    list->count++;
    return 0;
}

void remove_tuples_by_length(TupleList *list, size_t k) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }

    size_t write_index = 0;

    for (size_t read_index = 0; read_index < list->count; read_index++) {
        if (list->tuples[read_index].length != k) {
            if (write_index != read_index) {
                list->tuples[write_index] = list->tuples[read_index];
            }
            write_index++;
        } else {
            free(list->tuples[read_index].data);
            list->tuples[read_index].data = NULL;
        }
    }

    list->count = write_index;
}

void free_tuple_list(TupleList *list) {
    if (list == NULL) {
        return;
    }

    if (list->tuples != NULL) {
        for (size_t i = 0; i < list->count; i++) {
            free(list->tuples[i].data);
            list->tuples[i].data = NULL;
        }
        free(list->tuples);
        list->tuples = NULL;
    }

    list->count = 0;
    list->capacity = 0;
}

void print_tuples(const TupleList *list) {
    if (list == NULL || list->tuples == NULL) {
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        printf("(%s, len=%zu) ", list->tuples[i].data, list->tuples[i].length);
    }
    printf("\n");
}

int main(void) {
    TupleList list;

    if (init_tuple_list(&list, 10) != 0) {
        fprintf(stderr, "Failed to initialize tuple list\n");
        return EXIT_FAILURE;
    }

    if (add_tuple(&list, "hello") != 0 ||
        add_tuple(&list, "hi") != 0 ||
        add_tuple(&list, "world") != 0 ||
        add_tuple(&list, "abc") != 0 ||
        add_tuple(&list, "test") != 0 ||
        add_tuple(&list, "a") != 0) {
        fprintf(stderr, "Failed to add tuples\n");
        free_tuple_list(&list);
        return EXIT_FAILURE;
    }

    printf("Original tuples: ");
    print_tuples(&list);

    size_t k = 5;
    remove_tuples_by_length(&list, k);

    printf("After removing length %zu: ", k);
    print_tuples(&list);

    free_tuple_list(&list);

    return EXIT_SUCCESS;
}