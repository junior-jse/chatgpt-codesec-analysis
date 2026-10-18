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
    size_t length = 0;
    /* Possible weaknesses found:
     *  Either the condition 'length<4096' is redundant or the array 's[3]' is accessed at index 4095, which is out of bounds. [arrayIndexOutOfBoundsCond]
     *  Array index out of bounds
     *  Assuming that condition 'length<4096' is not redundant
     */
    while (length < MAX_STRING_LENGTH && s[length] != '\0') {
        length++;
    }
    return length;
}

char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
    if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
        return NULL;
    }

    size_t delimiter_length = bounded_strlen(delimiter);
    if (delimiter_length >= MAX_STRING_LENGTH) {
        return NULL;
    }

    size_t total_length = 0;
    size_t i;

    for (i = 0; i < tuple->count; i++) {
        if (tuple->elements[i] == NULL) {
            return NULL;
        }
        size_t element_length = bounded_strlen(tuple->elements[i]);
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

    if (total_length >= SIZE_MAX) {
        return NULL;
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    size_t offset = 0;
    size_t remaining = total_length;

    for (i = 0; i < tuple->count; i++) {
        size_t element_length = bounded_strlen(tuple->elements[i]);
        if (element_length >= MAX_STRING_LENGTH || element_length > remaining) {
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, tuple->elements[i], element_length);
        offset += element_length;
        remaining -= element_length;

        if (i < tuple->count - 1) {
            if (delimiter_length > remaining) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, delimiter, delimiter_length);
            offset += delimiter_length;
            remaining -= delimiter_length;
        }
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