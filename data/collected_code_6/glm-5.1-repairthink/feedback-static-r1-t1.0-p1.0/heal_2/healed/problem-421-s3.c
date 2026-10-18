#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STR_LEN 65536

char* tuple_concat(const char** tuple, size_t size, const char* delimiter) {
    if (tuple == NULL || delimiter == NULL || size == 0) {
        return NULL;
    }

    size_t total_len = 0;
    size_t delim_len = strnlen(delimiter, MAX_STR_LEN);

    for (size_t i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        size_t str_len = strnlen(tuple[i], MAX_STR_LEN);
        if (total_len > SIZE_MAX - str_len) {
            return NULL;
        }
        total_len += str_len;
        if (i < size - 1) {
            if (total_len > SIZE_MAX - delim_len) {
                return NULL;
            }
            total_len += delim_len;
        }
    }

    if (total_len == SIZE_MAX) {
        return NULL;
    }

    char* result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    char* ptr = result;
    size_t remaining = total_len + 1;

    for (size_t i = 0; i < size; i++) {
        size_t str_len = strnlen(tuple[i], MAX_STR_LEN);
        size_t to_copy = str_len < remaining ? str_len : remaining - 1;
        memcpy(ptr, tuple[i], to_copy);
        ptr += to_copy;
        remaining -= to_copy;

        if (i < size - 1) {
            size_t delim_to_copy = delim_len < remaining ? delim_len : remaining - 1;
            memcpy(ptr, delimiter, delim_to_copy);
            ptr += delim_to_copy;
            remaining -= delim_to_copy;
        }
    }
    *ptr = '\0';

    return result;
}

int main(void) {
    const char* tuple[] = {"C", "is", "a", "powerful", "language"};
    size_t size = sizeof(tuple) / sizeof(tuple[0]);
    const char* delimiter = "-";

    char* concatenated = tuple_concat(tuple, size, delimiter);
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
        concatenated = NULL;
    }

    return 0;
}