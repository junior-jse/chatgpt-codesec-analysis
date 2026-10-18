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

        size_t src_len = strnlen(list[i], size > 0 ? size * 256 : 256);
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

        if (dest_size > 0) {
            memcpy(dest, list[i], src_len);
            dest[src_len] = '\0';
        }
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