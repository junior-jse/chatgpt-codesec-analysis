#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TUPLE_NAME_SIZE 50

typedef struct {
    int id;
    char name[TUPLE_NAME_SIZE];
    double value;
} Tuple;

static int tuple_set(Tuple *tuple, int id, const char *name, double value) {
    size_t name_length;

    if (tuple == NULL || name == NULL) {
        return -1;
    }

    name_length = strlen(name);
    if (name_length >= TUPLE_NAME_SIZE) {
        return -1;
    }

    tuple->id = id;
    memcpy(tuple->name, name, name_length + 1);
    tuple->name[TUPLE_NAME_SIZE - 1] = '\0';
    tuple->value = value;

    return 0;
}

char *flatten_tuple_list(const Tuple *tuples, size_t count) {
    if (tuples == NULL || count == 0) {
        char *empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    size_t total_size = 1;

    for (size_t i = 0; i < count; i++) {
        if (memchr(tuples[i].name, '\0', TUPLE_NAME_SIZE) == NULL) {
            return NULL;
        }

        int needed = snprintf(NULL, 0, "(%d,%.*s,%.2f)",
                              tuples[i].id,
                              TUPLE_NAME_SIZE - 1,
                              tuples[i].name,
                              tuples[i].value);
        if (needed < 0) {
            return NULL;
        }
        if ((size_t)needed > SIZE_MAX - total_size - 1) {
            return NULL;
        }
        total_size += (size_t)needed + 1;
    }

    char *result = malloc(total_size);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t offset = 0;

    for (size_t i = 0; i < count; i++) {
        int written = snprintf(result + offset, total_size - offset,
                               "(%d,%.*s,%.2f)",
                               tuples[i].id,
                               TUPLE_NAME_SIZE - 1,
                               tuples[i].name,
                               tuples[i].value);
        if (written < 0 || (size_t)written >= total_size - offset) {
            free(result);
            return NULL;
        }
        offset += (size_t)written;

        if (i < count - 1) {
            if (offset + 1 >= total_size) {
                free(result);
                return NULL;
            }
            result[offset] = ',';
            offset++;
            result[offset] = '\0';
        }
    }

    return result;
}

int main(void) {
    Tuple tuples[3];
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);

    memset(tuples, 0, sizeof(tuples));

    if (tuple_set(&tuples[0], 1, "apple", 2.5) != 0 ||
        tuple_set(&tuples[1], 2, "banana", 1.75) != 0 ||
        tuple_set(&tuples[2], 3, "cherry", 3.0) != 0) {
        fprintf(stderr, "Error: Failed to initialize tuples\n");
        return EXIT_FAILURE;
    }

    char *flattened = flatten_tuple_list(tuples, count);

    if (flattened == NULL) {
        fprintf(stderr, "Error: Failed to flatten tuple list\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", flattened);
    free(flattened);

    return EXIT_SUCCESS;
}