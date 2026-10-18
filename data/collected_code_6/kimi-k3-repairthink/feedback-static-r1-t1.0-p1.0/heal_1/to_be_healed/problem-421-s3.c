#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char **elements;
    size_t count;
} Tuple;

char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
    if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
        return NULL;
    }

    size_t total_length = 0;
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    size_t delimiter_length = strlen(delimiter);
    size_t i;

    for (i = 0; i < tuple->count; i++) {
        if (tuple->elements[i] == NULL) {
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        total_length += strlen(tuple->elements[i]);
        if (i < tuple->count - 1) {
            total_length += delimiter_length;
        }
    }

    char *result = malloc(total_length + 1);
    if (result == NULL) {
        return NULL;
    }

    result[0] = '\0';
    size_t offset = 0;

    for (i = 0; i < tuple->count; i++) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t element_length = strlen(tuple->elements[i]);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result + offset, tuple->elements[i], element_length);
        offset += element_length;

        if (i < tuple->count - 1) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result + offset, delimiter, delimiter_length);
            offset += delimiter_length;
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