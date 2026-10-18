#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

Tuple* string_list_to_tuple(char **list, size_t count) {
    if (list == NULL) {
        return NULL;
    }

    Tuple *tuple = (Tuple*)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->items = (char**)malloc(count * sizeof(char*));
    if (tuple->items == NULL) {
        free(tuple);
        return NULL;
    }

    tuple->count = count;

    for (size_t i = 0; i < count; i++) {
        if (list[i] == NULL) {
            tuple->items[i] = NULL;
        } else {
            size_t len = strlen(list[i]) + 1;
            tuple->items[i] = (char*)malloc(len * sizeof(char));
            if (tuple->items[i] == NULL) {
                for (size_t j = 0; j < i; j++) {
                    free(tuple->items[j]);
                }
                free(tuple->items);
                free(tuple);
                return NULL;
            }
            memcpy(tuple->items[i], list[i], len);
        }
    }

    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple != NULL) {
        if (tuple->items != NULL) {
            for (size_t i = 0; i < tuple->count; i++) {
                free(tuple->items[i]);
            }
            free(tuple->items);
        }
        free(tuple);
    }
}

int main(void) {
    char *list[] = {"apple", "banana", "cherry"};
    size_t count = sizeof(list) / sizeof(list[0]);

    Tuple *tuple = string_list_to_tuple(list, count);
    if (tuple == NULL) {
        fprintf(stderr, "Failed to create tuple\n");
        return 1;
    }

    for (size_t i = 0; i < tuple->count; i++) {
        if (tuple->items[i] != NULL) {
            printf("(%s)\n", tuple->items[i]);
        }
    }

    free_tuple(tuple);

    return 0;
}