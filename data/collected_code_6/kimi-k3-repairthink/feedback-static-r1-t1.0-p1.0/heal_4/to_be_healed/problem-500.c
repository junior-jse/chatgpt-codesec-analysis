#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

static size_t bounded_strlen(const char* s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static int append_bytes(char* dest, size_t dest_capacity, size_t* offset, const char* src, size_t len) {
    size_t remaining;

    if (dest == NULL || offset == NULL || *offset > dest_capacity) {
        return -1;
    }

    remaining = dest_capacity - *offset;

    if (len == 0) {
        return 0;
    }

    if (src == NULL || len >= remaining) {
        return -1;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(dest + *offset, src, len);
    *offset += len;
    return 0;
}

char* concatenate_strings(const char** list, size_t count, const char* separator) {
    if (list == NULL || count == 0) {
        char* empty = malloc(1);
        if (empty != NULL) {
            empty[0] = '\0';
        }
        return empty;
    }

    size_t total_len = 0;
    size_t sep_len = 0;

    if (separator != NULL) {
        sep_len = bounded_strlen(separator, MAX_STRING_LENGTH);
    }

    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = bounded_strlen(list[i], MAX_STRING_LENGTH);
            if (len > SIZE_MAX - total_len) {
                return NULL;
            }
            total_len += len;
        }
        if (i < count - 1 && separator != NULL) {
            if (sep_len > SIZE_MAX - total_len) {
                return NULL;
            }
            total_len += sep_len;
        }
    }

    if (total_len == SIZE_MAX) {
        return NULL;
    }

    size_t capacity = total_len + 1;
    char* result = malloc(capacity);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;

    for (size_t i = 0; i < count; i++) {
        if (list[i] != NULL) {
            size_t len = bounded_strlen(list[i], MAX_STRING_LENGTH);
            if (append_bytes(result, capacity, &offset, list[i], len) != 0) {
                free(result);
                return NULL;
            }
        }
        if (i < count - 1 && separator != NULL && sep_len > 0) {
            if (append_bytes(result, capacity, &offset, separator, sep_len) != 0) {
                free(result);
                return NULL;
            }
        }
    }

    if (offset >= capacity) {
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
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", result);
    free(result);

    return EXIT_SUCCESS;
}