#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} StringTuple;

StringTuple *string_list_to_tuple(char **list, size_t count) {
    if (list == NULL || count == 0) {
        return NULL;
    }
    
    StringTuple *tuple = malloc(sizeof(StringTuple));
    if (tuple == NULL) {
        return NULL;
    }
    
    tuple->items = malloc(count * sizeof(char *));
    if (tuple->items == NULL) {
        free(tuple);
        return NULL;
    }
    
    tuple->count = count;
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(tuple->items[j]);
            }
            free(tuple->items);
            free(tuple);
            return NULL;
        }
        
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len = strlen(list[i]) + 1;
        tuple->items[i] = malloc(len);
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

void free_tuple(StringTuple *tuple) {
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
    char *string_list[] = {"apple", "banana", "cherry", "date"};
    size_t count = sizeof(string_list) / sizeof(string_list[0]);
    
    StringTuple *tuple = string_list_to_tuple(string_list, count);
    if (tuple == NULL) {
        fprintf(stderr, "Failed to convert list to tuple\n");
        return EXIT_FAILURE;
    }
    
    printf("Tuple contents:\n");
    for (size_t i = 0; i < tuple->count; i++) {
        printf("[%zu]: %s\n", i, tuple->items[i]);
    }
    
    free_tuple(tuple);
    return EXIT_SUCCESS;
}