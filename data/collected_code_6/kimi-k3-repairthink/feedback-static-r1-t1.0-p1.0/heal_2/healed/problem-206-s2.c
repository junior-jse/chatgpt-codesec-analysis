#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 65536

static size_t bounded_length(const char* s, size_t max_len) {
    size_t len = 0;
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

char* concatenate_adjacent(const char* tuple[], int size) {
    if (size <= 0 || tuple == NULL) {
        return NULL;
    }

    size_t total_len = 0;
    int i;

    for (i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            return NULL;
        }
        size_t len = bounded_length(tuple[i], MAX_STRING_LENGTH);
        size_t multiplier = (i > 0 && i < size - 1) ? 2 : 1;
        if (len > (SIZE_MAX - total_len) / multiplier) {
            return NULL;
        }
        total_len += multiplier * len;
    }

    if (total_len >= SIZE_MAX) {
        return NULL;
    }

    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;

    for (i = 0; i < size - 1; i++) {
        size_t len = bounded_length(tuple[i], MAX_STRING_LENGTH);
        size_t next_len = bounded_length(tuple[i + 1], MAX_STRING_LENGTH);

        if (offset > total_len || len > total_len - offset) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i], len);
        offset += len;

        if (offset > total_len || next_len > total_len - offset) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i + 1], next_len);
        offset += next_len;
    }

    result[offset] = '\0';

    return result;
}

int main(void) {
    const char* tuple[] = {"Hello", "World", "C", "Programming"};
    int size = (int)(sizeof(tuple) / sizeof(tuple[0]));

    char* concatenated = concatenate_adjacent(tuple, size);

    if (concatenated != NULL) {
        printf("%s\n", concatenated);
        free(concatenated);
        concatenated = NULL;
    } else {
        fprintf(stderr, "Memory allocation failed or invalid input\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}