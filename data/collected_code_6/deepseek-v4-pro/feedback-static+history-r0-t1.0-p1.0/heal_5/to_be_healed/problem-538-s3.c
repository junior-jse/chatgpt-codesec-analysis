#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t length;
} Tuple;

Tuple string_list_to_tuple(char **list, size_t size) {
    Tuple tuple;
    tuple.length = 0;
    tuple.items = NULL;

    if (list == NULL || size == 0) {
        return tuple;
    }

    tuple.items = (char **)malloc(size * sizeof(char *));
    if (tuple.items == NULL) {
        return tuple;
    }

    tuple.length = size;

    for (size_t i = 0; i < size; i++) {
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(tuple.items[j]);
            }
            free(tuple.items);
            tuple.items = NULL;
            tuple.length = 0;
            return tuple;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t src_len = strlen(list[i]);
        size_t dest_size = src_len + 1;

        char *dest = (char *)malloc(dest_size);
        if (dest == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(tuple.items[j]);
            }
            free(tuple.items);
            tuple.items = NULL;
            tuple.length = 0;
            return tuple;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(dest, list[i], src_len);
        dest[src_len] = '\0';
        tuple.items[i] = dest;
    }

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
    char *str_list[] = {"hello", "world", "tuple", "example"};
    size_t size = sizeof(str_list) / sizeof(str_list[0]);

    Tuple tuple = string_list_to_tuple(str_list, size);
    if (tuple.items == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return EXIT_FAILURE;
    }

    printf("Tuple contents:\n");
    for (size_t i = 0; i < tuple.length; i++) {
        printf("  [%zu] %s\n", i, tuple.items[i]);
    }

    free_tuple(&tuple);
    return EXIT_SUCCESS;
}