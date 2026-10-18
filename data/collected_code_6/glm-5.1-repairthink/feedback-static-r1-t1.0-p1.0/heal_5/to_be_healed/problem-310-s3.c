#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

typedef struct {
    char* first;
    char* second;
} Tuple;

void free_tuple(Tuple* tuple) {
    if (tuple != NULL) {
        free(tuple->first);
        free(tuple->second);
        free(tuple);
    }
}

Tuple* string_to_tuple(const char* str, size_t str_len, const char* delimiter, size_t delim_len) {
    if (str == NULL || delimiter == NULL || delim_len == 0) {
        return NULL;
    }

    Tuple* tuple = malloc(sizeof(Tuple));
    if (tuple == NULL) {
        return NULL;
    }
    
    tuple->first = NULL;
    tuple->second = NULL;

    const char* delim_pos = NULL;
    if (delim_len <= str_len) {
        for (size_t i = 0; i <= str_len - delim_len; ++i) {
            if (memcmp(str + i, delimiter, delim_len) == 0) {
                delim_pos = str + i;
                break;
            }
        }
    }

    if (delim_pos == NULL) {
        free_tuple(tuple);
        return NULL;
    }

    size_t first_len = delim_pos - str;
    if (first_len > SIZE_MAX - 1) {
        free_tuple(tuple);
        return NULL;
    }

    tuple->first = malloc(first_len + 1);
    if (tuple->first == NULL) {
        free_tuple(tuple);
        return NULL;
    }
    
    for (size_t i = 0; i < first_len; ++i) {
        tuple->first[i] = str[i];
    }
    tuple->first[first_len] = '\0';

    const char* second_start = delim_pos + delim_len;
    size_t second_len = str_len - (second_start - str);
    
    if (second_len > SIZE_MAX - 1) {
        free_tuple(tuple);
        return NULL;
    }

    tuple->second = malloc(second_len + 1);
    if (tuple->second == NULL) {
        free_tuple(tuple);
        return NULL;
    }
    
    for (size_t i = 0; i < second_len; ++i) {
        tuple->second[i] = second_start[i];
    }
    tuple->second[second_len] = '\0';

    return tuple;
}

int main(void) {
    const char* input = "key=value";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t input_len = strlen(input);
    const char* delimiter = "=";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t delim_len = strlen(delimiter);
    Tuple* my_tuple = string_to_tuple(input, input_len, delimiter, delim_len);

    if (my_tuple != NULL) {
        printf("First: %s\n", my_tuple->first);
        printf("Second: %s\n", my_tuple->second);
        free_tuple(my_tuple);
    }

    return 0;
}