#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    char* first;
    char* second;
} Tuple;

Tuple* string_to_tuple(const char* str, const char* delimiter) {
    if (str == NULL || delimiter == NULL) {
        return NULL;
    }

    size_t delim_len = strlen(delimiter);
    if (delim_len == 0) {
        return NULL;
    }

    const char* delim_pos = strstr(str, delimiter);
    if (delim_pos == NULL) {
        return NULL;
    }

    Tuple* tuple = malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    size_t first_len = delim_pos - str;
    if (first_len > SIZE_MAX - 1 || first_len > INT_MAX) {
        free(tuple);
        return NULL;
    }

    tuple->first = malloc(first_len + 1);
    if (tuple->first == NULL) {
        free(tuple);
        return NULL;
    }
    snprintf(tuple->first, first_len + 1, "%.*s", (int)first_len, str);

    const char* second_start = delim_pos + delim_len;
    size_t second_len = strlen(second_start);
    
    if (second_len > SIZE_MAX - 1) {
        free(tuple->first);
        free(tuple);
        return NULL;
    }

    tuple->second = malloc(second_len + 1);
    if (tuple->second == NULL) {
        free(tuple->first);
        free(tuple);
        return NULL;
    }
    snprintf(tuple->second, second_len + 1, "%s", second_start);

    return tuple;
}

void free_tuple(Tuple* tuple) {
    if (tuple != NULL) {
        free(tuple->first);
        free(tuple->second);
        free(tuple);
    }
}

int main(void) {
    const char* input = "key=value";
    Tuple* my_tuple = string_to_tuple(input, "=");

    if (my_tuple != NULL) {
        printf("First: %s\n", my_tuple->first);
        printf("Second: %s\n", my_tuple->second);
        free_tuple(my_tuple);
    }

    return 0;
}