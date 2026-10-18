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

char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
    size_t delimiter_length;
    size_t total_length = 0;
    size_t i;
    size_t offset;
    size_t remaining;
    char *result;

    if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
        return NULL;
    }

    delimiter_length = bounded_strlen(delimiter);
    if (delimiter_length >= MAX_STRING_LENGTH) {
        return NULL;
    }

    for (i = 0; i < tuple->count; i++) {
        size_t element_length;

        if (tuple->elements[i] == NULL) {
            return NULL;
        }

        element_length = bounded_strlen(tuple->elements[i]);
        if (element_length >= MAX_STRING_LENGTH) {
            return NULL;
        }
        if (element_length > SIZE_MAX - total_length) {
            return NULL;
        }
        total_length += element_length;

        if (i < tuple->count - 1) {
            if (delimiter_length > SIZE_MAX - total_length) {
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    if (total_length >= SIZE_MAX - 1) {
        return NULL;
    }

    result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    offset = 0;
    remaining = total_length;

    for (i = 0; i < tuple->count; i++) {
        size_t element_length = bounded_strlen(tuple->elements[i]);

        if (element_length >= MAX_STRING_LENGTH ||
            element_length > remaining ||
            offset > total_length ||
            element_length > total_length - offset) {
            free(result);
            return NULL;
        }
        memcpy(result + offset, tuple->elements[i], element_length);
        offset += element_length;
        remaining -= element_length;

        if (i < tuple->count - 1) {
            if (delimiter_length > remaining ||
                offset > total_length ||
                delimiter_length > total_length - offset) {
                free(result);
                return NULL;
            }
            memcpy(result + offset, delimiter, delimiter_length);
            offset += delimiter_length;
            remaining -= delimiter_length;
        }
    }

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