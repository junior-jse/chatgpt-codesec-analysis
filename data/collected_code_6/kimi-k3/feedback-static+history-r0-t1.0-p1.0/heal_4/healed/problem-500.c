#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static size_t safe_strnlen(const char* s, size_t max_len) {
    if (s == NULL) {
        return 0;
    }
    
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static bool safe_memcpy(void* dest, size_t dest_size, size_t offset, const void* src, size_t n) {
    if (dest == NULL || src == NULL) {
        return false;
    }
    if (offset > dest_size || n > dest_size - offset) {
        return false;
    }
    if (n > 0) {
        memcpy((char*)dest + offset, src, n);
    }
    return true;
}

char* concatenate_strings(const char** list, size_t count, const char* separator) {
    if (list == NULL || count == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    const size_t MAX_STRING_LEN = 1024 * 1024;
    size_t total_len = 0;
    size_t sep_len = 0;
    
    if (separator != NULL) {
        sep_len = safe_strnlen(separator, MAX_STRING_LEN);
        if (sep_len == MAX_STRING_LEN && separator[MAX_STRING_LEN] != '\0') {
            return NULL;
        }
    }
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = safe_strnlen(list[i], MAX_STRING_LEN);
            if (len == MAX_STRING_LEN && list[i][MAX_STRING_LEN] != '\0') {
                return NULL;
            }
            
            if (total_len > SIZE_MAX - len) {
                return NULL;
            }
            total_len += len;
        }
        
        if (i < count - 1) {
            if (total_len > SIZE_MAX - sep_len) {
                return NULL;
            }
            total_len += sep_len;
        }
    }

    if (total_len == SIZE_MAX) {
        return NULL;
    }

    char* result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = safe_strnlen(list[i], MAX_STRING_LEN);
            if (len > 0) {
                if (!safe_memcpy(result, total_len + 1, offset, list[i], len)) {
                    free(result);
                    return NULL;
                }
                offset += len;
            }
        }
        
        if (i < count - 1 && separator != NULL && sep_len > 0) {
            if (!safe_memcpy(result, total_len + 1, offset, separator, sep_len)) {
                free(result);
                return NULL;
            }
            offset += sep_len;
        }
    }
    
    if (offset > total_len) {
        free(result);
        return NULL;
    }
    
    result[offset] = '\0';
    return result;
}

int main(void) {
    const char* words[] = {"Hello", "World", "C", "Programming"};
    size_t count = sizeof(words) / sizeof(words[0]);
    
    char* result = concatenate_strings(words, count, " ");
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("%s\n", result);
    free(result);
    
    return EXIT_SUCCESS;
}