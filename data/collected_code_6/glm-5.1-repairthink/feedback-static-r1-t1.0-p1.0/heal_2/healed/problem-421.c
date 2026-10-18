#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char* concatenate_tuple(const char* tuple[], size_t size, const char* delimiter) {
    if (tuple == NULL || delimiter == NULL || size == 0) {
        return NULL;
    }

    size_t total_len = 0;
    size_t delim_len = strnlen(delimiter, SIZE_MAX);

    for (size_t i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        total_len += strnlen(tuple[i], SIZE_MAX);
        if (i < size - 1) {
            total_len += delim_len;
        }
    }

    size_t buffer_size = total_len + 1;
    char* result = (char*)malloc(buffer_size);
    if (result == NULL) {
        return NULL;
    }

    size_t remaining = buffer_size;
    char* ptr = result;

    for (size_t i = 0; i < size; i++) {
        size_t len = strnlen(tuple[i], remaining);
        if (len > 0) {
            memcpy(ptr, tuple[i], len);
            ptr += len;
            remaining -= len;
        }
        if (i < size - 1) {
            size_t d_len = strnlen(delimiter, remaining);
            if (d_len > 0) {
                memcpy(ptr, delimiter, d_len);
                ptr += d_len;
                remaining -= d_len;
            }
        }
    }

    if (remaining == 0) {
        result[buffer_size - 1] = '\0';
    } else {
        *ptr = '\0';
    }

    return result;
}

int main(void) {
    const char* my_tuple[] = {"apple", "banana", "cherry"};
    size_t tuple_size = sizeof(my_tuple) / sizeof(my_tuple[0]);
    const char* delimiter = ", ";

    char* concatenated = concatenate_tuple(my_tuple, tuple_size, delimiter);
    
    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
        concatenated = NULL;
    }

    return 0;
}