#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char **items;
    size_t count;
} StringTuple;

void free_string_tuple(StringTuple *tuple);

StringTuple *string_list_to_tuple(char **list, size_t count) {
    if (list == NULL || count == 0) {
        return NULL;
    }

    if (count > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    StringTuple *tuple = malloc(sizeof(StringTuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->items = calloc(count, sizeof(char *));
    if (tuple->items == NULL) {
        free(tuple);
        return NULL;
    }

    tuple->count = count;

    for (size_t i = 0; i < count; i++) {
        if (list[i] == NULL) {
            tuple->items[i] = NULL;
            continue;
        }

        size_t len = strnlen(list[i], MAX_STRING_LENGTH);
        size_t buf_size = len + 1;

        tuple->items[i] = malloc(buf_size);
        if (tuple->items[i] == NULL) {
            free_string_tuple(tuple);
            return NULL;
        }

        int written = snprintf(tuple->items[i], buf_size, "%.*s", (int)len, list[i]);
        if (written < 0 || (size_t)written >= buf_size) {
            free_string_tuple(tuple);
            return NULL;
        }
    }

    return tuple;
}

void free_string_tuple(StringTuple *tuple) {
    if (tuple == NULL) {
        return;
    }

    if (tuple->items != NULL) {
        for (size_t i = 0; i < tuple->count; i++) {
            free(tuple->items[i]);
        }
        free(tuple->items);
    }

    free(tuple);
}

int main(void) {
    char *string_list[] = {
        "apple",
        "banana",
        "cherry",
        "date"
    };

    size_t count = sizeof(string_list) / sizeof(string_list[0]);

    StringTuple *tuple = string_list_to_tuple(string_list, count);
    if (tuple == NULL) {
        fprintf(stderr, "Failed to convert string list to tuple\n");
        return EXIT_FAILURE;
    }

    printf("Tuple contents:\n");
    for (size_t i = 0; i < tuple->count; i++) {
        if (tuple->items[i] != NULL) {
            printf("  [%zu]: %s\n", i, tuple->items[i]);
        }
    }

    free_string_tuple(tuple);

    return EXIT_SUCCESS;
}