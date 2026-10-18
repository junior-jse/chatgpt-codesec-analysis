#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    memcpy(copy, str, len + 1);

    size_t capacity = 0;
    char *token = strtok(copy, ",");

    while (token != NULL) {
        while (*token == ' ') {
            token++;
        }

        if (*token == '\0') {
            token = strtok(NULL, ",");
            continue;
        }

        size_t tok_len = strlen(token);
        char *end = token + tok_len - 1;
        while (end > token && *end == ' ') {
            end--;
        }
        *(end + 1) = '\0';
        tok_len = (size_t)(end - token + 1);

        if (tuple.count >= capacity) {
            size_t new_capacity = (capacity == 0) ? 4 : capacity * 2;
            if (new_capacity > SIZE_MAX / sizeof(char *)) {
                for (size_t i = 0; i < tuple.count; i++) {
                    free(tuple.elements[i]);
                }
                free(tuple.elements);
                free(copy);
                tuple.elements = NULL;
                tuple.count = 0;
                return tuple;
            }
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

        if (tok_len == SIZE_MAX) {
            for (size_t i = 0; i < tuple.count; i++) {
                free(tuple.elements[i]);
            }
            free(tuple.elements);
            free(copy);
            tuple.elements = NULL;
            tuple.count = 0;
            return tuple;
        }
        tuple.elements[tuple.count] = (char *)malloc(tok_len + 1);
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
        memcpy(tuple.elements[tuple.count], token, tok_len + 1);
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