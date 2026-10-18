#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    const char **elements;
    size_t count;
} Tuple;

static size_t bounded_strlen(const char *s) {
    const char *end;

    if (s == NULL) {
        return 0;
    }

    end = memchr(s, '\0', MAX_STRING_LENGTH);
    if (end == NULL) {
        return MAX_STRING_LENGTH;
    }

    return (size_t)(end - s);
}

static int checked_memcpy(char *dest, size_t dest_size, size_t offset, const void *src, size_t n) {
    if (dest == NULL || (src == NULL && n > 0)) {
        return -1;
    }
    if (offset > dest_size) {
        return -1;
    }
    if (n > dest_size - offset) {
        return -1;
    }
    if (n > 0) {
        memcpy(dest + offset, src, n);
    }
    return 0;
}

char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
    size_t delimiter_length;
    size_t total_length = 0;
    size_t i;
    size_t offset;
    size_t buffer_size;
    size_t *lengths;
    char *result;

    if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
        return NULL;
    }

    delimiter_length = bounded_strlen(delimiter);
    if (delimiter_length >= MAX_STRING_LENGTH) {
        return NULL;
    }

    if (tuple->count > SIZE_MAX / sizeof(*lengths)) {
        return NULL;
    }

    lengths = malloc(tuple->count * sizeof(*lengths));
    if (lengths == NULL) {
        return NULL;
    }

    for (i = 0; i < tuple->count; i++) {
        if (tuple->elements[i] == NULL) {
            free(lengths);
            return NULL;
        }

        lengths[i] = bounded_strlen(tuple->elements[i]);
        if (lengths[i] >= MAX_STRING_LENGTH) {
            free(lengths);
            return NULL;
        }
        if (lengths[i] > SIZE_MAX - total_length) {
            free(lengths);
            return NULL;
        }
        total_length += lengths[i];

        if (i < tuple->count - 1) {
            if (delimiter_length > SIZE_MAX - total_length) {
                free(lengths);
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    if (total_length >= SIZE_MAX - 1) {
        free(lengths);
        return NULL;
    }

    buffer_size = total_length + 1;
    result = malloc(buffer_size);
    if (result == NULL) {
        free(lengths);
        return NULL;
    }

    offset = 0;

    for (i = 0; i < tuple->count; i++) {
        if (checked_memcpy(result, buffer_size, offset, tuple->elements[i], lengths[i]) != 0) {
            free(result);
            free(lengths);
            return NULL;
        }
        offset += lengths[i];

        if (i < tuple->count - 1) {
            if (checked_memcpy(result, buffer_size, offset, delimiter, delimiter_length) != 0) {
                free(result);
                free(lengths);
                return NULL;
            }
            offset += delimiter_length;
        }
    }

    free(lengths);

    if (offset != total_length) {
        free(result);
        return NULL;
    }

    result[offset] = '\0';
    return result;
}

int main(void) {
    const char *elements[] = {"apple", "banana", "cherry", "date"};
    Tuple tuple = {elements, sizeof(elements) / sizeof(elements[0])};
    const char *delimiter = ", ";
    char *result;

    result = concatenate_tuple(&tuple, delimiter);
    if (result == NULL) {
        fprintf(stderr, "Failed to concatenate tuple\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", result);
    free(result);

    return EXIT_SUCCESS;
}