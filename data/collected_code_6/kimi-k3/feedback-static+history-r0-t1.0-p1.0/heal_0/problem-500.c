#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* concatenate_strings(const char** list, size_t count, const char* separator) {
    if (list == NULL || count == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    size_t total_len = 0;
    size_t sep_len = (separator != NULL) ? strlen(separator) : 0;
    
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            total_len += strlen(list[i]);
        }
        if (i < count - 1) {
            total_len += sep_len;
        }
    }

    char* result = malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;
    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = strlen(list[i]);
            memcpy(result + offset, list[i], len);
            offset += len;
        }
        if (i < count - 1 && separator != NULL) {
            memcpy(result + offset, separator, sep_len);
            offset += sep_len;
        }
    }
    result[offset] = '\0';

    return result;
}

int main(void) {
    const char* words[] = {"Hello", "World", "C", "Programming"};
    size_t count = sizeof(words) / sizeof(words[0]);
    
    char* result = concatenate_strings(words, count, " ");
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }
    
    printf("%s\n", result);
    free(result);
    
    return EXIT_SUCCESS;
}