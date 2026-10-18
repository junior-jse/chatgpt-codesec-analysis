#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* first;
    char* second;
} Tuple;

Tuple* string_to_tuple(const char* str, const char* delimiter) {
    if (str == NULL || delimiter == NULL) {
        return NULL;
    }

    size_t str_len = strlen(str);
    size_t delim_len = strlen(delimiter);

    if (delim_len == 0) {
        return NULL;
    }

    const char* delim_pos = strstr(str, delimiter);
    if (delim_pos == NULL) {
        return NULL;
    }

    Tuple* tuple = (Tuple*)malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }

    size_t first_len = delim_pos - str;
    tuple->first = (char*)malloc(first_len + 1);
    if (tuple->first == NULL) {
        free(tuple);
        return NULL;
    }
    strncpy(tuple->first, str, first_len);
    tuple->first[first_len] = '\0';

    const char* second_start = delim_pos + delim_len;
    size_t second_len = str_len - (second_start - str);
    tuple->second = (char*)malloc(second_len + 1);
    if (tuple->second == NULL) {
        free(tuple->first);
        free(tuple);
        return NULL;
    }
    strncpy(tuple->second, second_start, second_len);
    tuple->second[second_len] = '\0';

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