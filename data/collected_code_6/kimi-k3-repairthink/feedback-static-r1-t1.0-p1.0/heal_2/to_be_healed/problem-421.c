#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TUPLE_JOIN_MAX_STRING_LENGTH 65535

typedef struct {
    char **elements;
    size_t count;
} Tuple;

char *tuple_join(const Tuple *tuple, const char *delimiter)
{
    size_t total_length;
    size_t delimiter_length;
    size_t i;
    size_t remaining;
    char *result;
    char *current;

    if (tuple == NULL || delimiter == NULL) {
        return NULL;
    }

    if (tuple->count == 0 || tuple->elements == NULL) {
        result = malloc(1);
        if (result != NULL) {
            result[0] = '\0';
        }
        return result;
    }

    delimiter_length = strnlen(delimiter, TUPLE_JOIN_MAX_STRING_LENGTH + 1);
    if (delimiter_length > TUPLE_JOIN_MAX_STRING_LENGTH) {
        return NULL;
    }

    total_length = 1;

    for (i = 0; i < tuple->count; i++) {
        size_t element_length;

        if (tuple->elements[i] == NULL) {
            return NULL;
        }

        element_length = strnlen(tuple->elements[i], TUPLE_JOIN_MAX_STRING_LENGTH + 1);
        if (element_length > TUPLE_JOIN_MAX_STRING_LENGTH) {
            return NULL;
        }

        if (total_length > SIZE_MAX - element_length) {
            return NULL;
        }
        total_length += element_length;

        if (i > 0) {
            if (total_length > SIZE_MAX - delimiter_length) {
                return NULL;
            }
            total_length += delimiter_length;
        }
    }

    result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    current = result;
    remaining = total_length;

    for (i = 0; i < tuple->count; i++) {
        size_t element_length;

        if (i > 0) {
            if (delimiter_length >= remaining) {
                free(result);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(current, delimiter, delimiter_length);
            current += delimiter_length;
            remaining -= delimiter_length;
        }

        element_length = strnlen(tuple->elements[i], TUPLE_JOIN_MAX_STRING_LENGTH + 1);
        if (element_length > TUPLE_JOIN_MAX_STRING_LENGTH) {
            free(result);
            return NULL;
        }

        if (element_length >= remaining) {
            free(result);
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(current, tuple->elements[i], element_length);
        current += element_length;
        remaining -= element_length;
    }

    *current = '\0';
    return result;
}

int main(void)
{
    char *data[] = {"apple", "banana", "cherry", "date"};
    Tuple tuple = {data, 4};
    char *joined;

    joined = tuple_join(&tuple, ", ");
    if (joined == NULL) {
        fprintf(stderr, "Failed to join tuple elements\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", joined);
    free(joined);

    return EXIT_SUCCESS;
}