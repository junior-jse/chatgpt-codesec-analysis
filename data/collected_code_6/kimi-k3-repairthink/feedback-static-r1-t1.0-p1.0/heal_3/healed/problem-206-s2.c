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
    if (tuple == NULL || size <= 0) {
        return NULL;
    }

    if ((size_t)size > SIZE_MAX / sizeof(size_t)) {
        return NULL;
    }

    size_t* lengths = (size_t*)malloc((size_t)size * sizeof(size_t));
    if (lengths == NULL) {
        return NULL;
    }

    size_t total_len = 0;
    int i;

    for (i = 0; i < size; i++) {
        if (tuple[i] == NULL) {
            free(lengths);
            return NULL;
        }
        lengths[i] = bounded_length(tuple[i], MAX_STRING_LENGTH);
        size_t multiplier = (i > 0 && i < size - 1) ? (size_t)2 : (size_t)1;
        if (lengths[i] > (SIZE_MAX - total_len) / multiplier) {
            free(lengths);
            return NULL;
        }
        total_len += multiplier * lengths[i];
    }

    if (total_len >= SIZE_MAX) {
        free(lengths);
        return NULL;
    }

    char* result = (char*)malloc(total_len + 1);
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    size_t offset = 0;
    size_t remaining = total_len;

    for (i = 0; i < size - 1; i++) {
        size_t len = lengths[i];
        size_t next_len = lengths[i + 1];

        if (len > remaining) {
            free(lengths);
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i], len);
        offset += len;
        remaining -= len;

        if (next_len > remaining) {
            free(lengths);
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple[i + 1], next_len);
        offset += next_len;
        remaining -= next_len;
    }

    free(lengths);
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