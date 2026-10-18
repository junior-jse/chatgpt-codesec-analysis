#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **items;
    size_t count;
} Tuple;

Tuple *string_list_to_tuple(char **list, size_t count) {
    if (list == NULL || count == 0) {
        return NULL;
    }
    
    Tuple *tuple = malloc(sizeof(Tuple));
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
        strcpy(tuple->items[i], list[i]);
    }
    
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple == NULL) {
        return;
    }
    
    for (size_t i = 0; i < tuple->count; i++) {
        free(tuple->items[i]);
    }
    free(tuple->items);
    free(tuple);
}

int main(void) {
    char *list[] = {"apple", "banana", "cherry", "date"};
    size_t count = sizeof(list) / sizeof(list[0]);
    
    Tuple *tuple = string_list_to_tuple(list, count);
    if (tuple == NULL) {
        fprintf(stderr, "Failed to convert list to tuple\n");
        return 1;
    }
    
    printf("(");
    for (size_t i = 0; i < tuple->count; i++) {
        printf("\"%s\"", tuple->items[i]);
        if (i < tuple->count - 1) {
            printf(", ");
        }
    }
    printf(")\n");
    
    free_tuple(tuple);
    
    return 0;
}