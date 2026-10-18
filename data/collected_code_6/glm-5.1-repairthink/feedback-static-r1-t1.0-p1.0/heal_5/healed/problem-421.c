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
    size_t *lens = malloc(size * sizeof(size_t));
    
    if (lens == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            free(lens);
            return NULL;
        }
        lens[i] = strnlen(tuple[i], SIZE_MAX);
        
        if (lens[i] > SIZE_MAX - total_len) {
            free(lens);
            return NULL;
        }
        total_len += lens[i];
        
        if (i < size - 1) {
            if (delim_len > SIZE_MAX - total_len) {
                free(lens);
                return NULL;
            }
            total_len += delim_len;
        }
    }

    if (total_len >= SIZE_MAX) {
        free(lens);
        return NULL;
    }

    size_t buffer_size = total_len + 1;
    char* result = malloc(buffer_size);
    
    if (result == NULL) {
        free(lens);
        return NULL;
    }

    size_t offset = 0;

    for (size_t i = 0; i < size; i++) {
        if (lens[i] > 0) {
            if (offset + lens[i] > total_len) {
                free(lens);
                free(result);
                return NULL;
            }
            memcpy(result + offset, tuple[i], lens[i]);
            offset += lens[i];
        }
        if (i < size - 1 && delim_len > 0) {
            if (offset + delim_len > total_len) {
                free(lens);
                free(result);
                return NULL;
            }
            memcpy(result + offset, delimiter, delim_len);
            offset += delim_len;
        }
    }

    result[total_len] = '\0';
    free(lens);

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
    }

    return 0;
}