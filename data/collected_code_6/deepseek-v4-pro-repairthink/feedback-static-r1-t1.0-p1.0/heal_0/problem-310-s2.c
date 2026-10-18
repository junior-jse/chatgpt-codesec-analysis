#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **elements;
    size_t count;
} Tuple;

Tuple string_to_tuple(const char *str) {
    Tuple tuple;
    tuple.elements = NULL;
    tuple.count = 0;
    
    if (str == NULL) {
        return tuple;
    }
    
    size_t len = strlen(str);
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return tuple;
    }
    strcpy(copy, str);
    
    size_t capacity = 0;
    char *token = strtok(copy, ",");
    
    while (token != NULL) {
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        
        if (tuple.count >= capacity) {
            size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
            char **new_elements = (char **)realloc(tuple.elements, new_capacity * sizeof(char *));
            if (new_elements == NULL) {
                for (size_t i = 0; i < tuple.count; i++) {
                    free(tuple.elements[i]);
                }
                free(tuple.elements);
                free(copy);
                tuple.elements = NULL;
                tuple.count = 0;
                return tuple;
            }
            tuple.elements = new_elements;
            capacity = new_capacity;
        }
        
        tuple.elements[tuple.count] = (char *)malloc(strlen(token) + 1);
        if (tuple.elements[tuple.count] == NULL) {
            for (size_t i = 0; i < tuple.count; i++) {
                free(tuple.elements[i]);
            }
            free(tuple.elements);
            free(copy);
            tuple.elements = NULL;
            tuple.count = 0;
            return tuple;
        }
        strcpy(tuple.elements[tuple.count], token);
        tuple.count++;
        token = strtok(NULL, ",");
    }
    
    free(copy);
    return tuple;
}

void free_tuple(Tuple *tuple) {
    if (tuple == NULL || tuple->elements == NULL) {
        return;
    }
    for (size_t i = 0; i < tuple->count; i++) {
        free(tuple->elements[i]);
    }
    free(tuple->elements);
    tuple->elements = NULL;
    tuple->count = 0;
}

int main(void) {
    const char *input = "apple, banana, cherry, date";
    Tuple tuple = string_to_tuple(input);
    
    if (tuple.elements != NULL) {
        printf("Tuple elements (%zu):\n", tuple.count);
        for (size_t i = 0; i < tuple.count; i++) {
            printf("  [%zu]: %s\n", i, tuple.elements[i]);
        }
        free_tuple(&tuple);
    } else {
        printf("Failed to convert string to tuple\n");
    }
    
    return 0;
}