#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char **items;
    size_t length;
} Tuple;

static size_t bounded_strlen(const char *str, size_t maxlen) {
    size_t i = 0;
    if (str == NULL) return 0;
    while (i < maxlen && str[i] != '\0') {
        i++;
    }
    return i;
}

Tuple string_list_to_tuple(char **list, size_t length) {
    Tuple tuple = { NULL, 0 };

    if (list == NULL || length == 0) {
        return tuple;
    }

    tuple.items = (char **)malloc(length * sizeof(char *));
    if (tuple.items == NULL) {
        return tuple;
    }
    tuple.length = length;

    size_t i;
    for (i = 0; i < length; i++) {
        size_t len = bounded_strlen(list[i], MAX_STRING_LENGTH);
        if (len == MAX_STRING_LENGTH) {
            goto cleanup;
        }

        size_t alloc_size = len + 1;
        tuple.items[i] = (char *)malloc(alloc_size);
        if (tuple.items[i] == NULL) {
            goto cleanup;
        }

        /* Possible weaknesses found:
         * Flawfinder strncpy: Easily used incorrectly; doesn't always \0-terminate or check for invalid pointers [MS-banned] (CWE-120). (risk 1, buffer)
         */
        strncpy(tuple.items[i], list[i], alloc_size);
        tuple.items[i][alloc_size - 1] = '\0';
    }

    return tuple;

cleanup:
    for (size_t j = 0; j < i; j++) {
        free(tuple.items[j]);
    }
    free(tuple.items);
    tuple.items = NULL;
    tuple.length = 0;
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple == NULL || tuple->items == NULL) {
        return;
    }

    for (size_t i = 0; i < tuple->length; i++) {
        free(tuple->items[i]);
    }
    free(tuple->items);
    tuple->items = NULL;
    tuple->length = 0;
}

int main(void) {
    char *list[] = {"hello", "world", "example", "tuple"};
    size_t length = sizeof(list) / sizeof(list[0]);

    Tuple tuple = string_list_to_tuple(list, length);

    if (tuple.items == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return 1;
    }

    printf("Tuple contents:\n");
    for (size_t i = 0; i < tuple.length; i++) {
        printf("[%zu] %s\n", i, tuple.items[i]);
    }

    free_tuple(&tuple);
    return 0;
}