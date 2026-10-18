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
    
    size_t len = strnlen(str, 4096);
    if (len == 4096) {
        return tuple;
    }
    
    char *copy = (char *)malloc(len + 1);
    if (copy == NULL) {
        return tuple;
    }
    
    if (len > 0) {
        memcpy(copy, str, len);
    }
    copy[len] = '\0';
    
    size_t capacity = 0;
    char *saveptr;
    char *token = strtok_r(copy, ",", &saveptr);
    
    while (token != NULL) {
        while (*token == ' ') token++;
        size_t token_len = strnlen(token, 4096);
        if (token_len == 4096) {
            token_len = 4095;
        }
        if (token_len == 0) {
            token = strtok_r(NULL, ",", &saveptr);
            continue;
        }
        char *end = token + token_len - 1;
        while (end > token && *end == ' ') end--;
        *(end + 1) = '\0';
        token_len = (size_t)(end + 1 - token);
        
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
        
        tuple.elements[tuple.count] = (char *)malloc(token_len + 1);
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
        
        if (token_len > 0) {
            memcpy(tuple.elements[tuple.count], token, token_len);
        }
        tuple.elements[tuple.count][token_len] = '\0';
        tuple.count++;
        token = strtok_r(NULL, ",", &saveptr);
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