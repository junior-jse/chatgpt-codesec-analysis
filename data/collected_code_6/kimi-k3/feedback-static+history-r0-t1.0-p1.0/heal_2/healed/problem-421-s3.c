#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    const char **elements;
    size_t count;
} Tuple;

static size_t safe_strlen(const char *s, size_t max_len) {
    size_t len = 0;
    if (s == NULL) {
        return 0;
    }
    while (len < max_len && s[len] != '\0') {
        len++;
    }
    return len;
}

static bool safe_memcpy(void *dest, size_t dest_size, size_t offset, const void *src, size_t n) {
    if (dest == NULL || src == NULL) {
        return false;
    }
    if (offset > dest_size || n > dest_size - offset) {
        return false;
    }
    if (n > 0) {
        memcpy((char *)dest + offset, src, n);
    }
    return true;
}

char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
    if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
        return NULL;
    }

    if (tuple->count > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    size_t total_length = 0;
    size_t delimiter_length = safe_strlen(delimiter, SIZE_MAX);
    size_t i;

    for (i = 0; i < tuple->count; i++) {
        if (tuple->elements[i] == NULL) {
            return NULL;
        }
        size_t element_length = safe_strlen(tuple->elements[i], SIZE_MAX);
        
        if (total_length > SIZE_MAX - element_length) {
            return NULL;
        }
        total_length += element_length;
        
        if (i < tuple->count - 1) {
            if (total_length > SIZE_MAX - delimiter_length) {
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    if (total_length == SIZE_MAX) {
        return NULL;
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;

    for (i = 0; i < tuple->count; i++) {
        size_t element_length = safe_strlen(tuple->elements[i], SIZE_MAX);
        
        if (!safe_memcpy(result, total_length + 1, offset, tuple->elements[i], element_length)) {
            free(result);
            return NULL;
        }
        offset += element_length;

        if (i < tuple->count - 1) {
            if (!safe_memcpy(result, total_length + 1, offset, delimiter, delimiter_length)) {
                free(result);
                return NULL;
            }
            offset += delimiter_length;
        }
    }

    if (offset > total_length) {
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

    char *result = concatenate_tuple(&tuple, delimiter);
    if (result == NULL) {
        fprintf(stderr, "Failed to concatenate tuple\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", result);
    free(result);

    return EXIT_SUCCESS;
}