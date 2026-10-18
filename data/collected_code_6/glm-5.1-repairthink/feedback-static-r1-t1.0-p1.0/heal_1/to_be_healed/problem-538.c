#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char **items;
    size_t length;
} Tuple;

Tuple *string_list_to_tuple(char **list, size_t length) {
    if (list == NULL || length == 0) {
        return NULL;
    }

    Tuple *tuple = malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    tuple->items = malloc(length * sizeof(char *));
    if (tuple->items == NULL) {
        free(tuple);
        return NULL;
    }

    tuple->length = length;

    for (size_t i = 0; i < length; i++) {
        if (list[i] == NULL) {
            tuple->items[i] = NULL;
            continue;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        tuple->items[i] = malloc(strlen(list[i]) + 1);
        if (tuple->items[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(tuple->items[j]);
            }
            free(tuple->items);
            free(tuple);
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strcpy: Does not check for buffer overflows when copying to destination [MS-banned] (CWE-120). Consider using snprintf, strcpy_s, or strlcpy (warning: strncpy easily misused). (risk 4, buffer)
         */
        strcpy(tuple->items[i], list[i]);
    }

    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple == NULL) {
        return;
    }
    for (size_t i = 0; i < tuple->length; i++) {
        free(tuple->items[i]);
    }
    free(tuple->items);
    free(tuple);
}

int main(void) {
    char *list[] = {"apple", "banana", "cherry"};
    size_t length = sizeof(list) / sizeof(list[0]);

    Tuple *tuple = string_list_to_tuple(list, length);
    if (tuple == NULL) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < tuple->length; i++) {
        if (tuple->items[i] != NULL) {
            printf("%s\n", tuple->items[i]);
        }
    }

    free_tuple(tuple);

    return EXIT_SUCCESS;
}