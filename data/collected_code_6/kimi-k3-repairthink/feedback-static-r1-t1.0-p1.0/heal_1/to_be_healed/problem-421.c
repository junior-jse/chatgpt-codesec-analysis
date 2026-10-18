#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char **elements;
    size_t count;
} Tuple;

char *tuple_join(const Tuple *tuple, const char *delimiter)
{
    size_t total_length;
    size_t delimiter_length;
    size_t i;
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

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    delimiter_length = strlen(delimiter);
    total_length = 1;

    for (i = 0; i < tuple->count; i++) {
        if (tuple->elements[i] == NULL) {
            return NULL;
        }
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        total_length += strlen(tuple->elements[i]);
        if (i > 0) {
            total_length += delimiter_length;
        }
    }

    result = malloc(total_length);
    if (result == NULL) {
        return NULL;
    }

    current = result;
    for (i = 0; i < tuple->count; i++) {
        size_t element_length;

        if (i > 0) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(current, delimiter, delimiter_length);
            current += delimiter_length;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        element_length = strlen(tuple->elements[i]);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(current, tuple->elements[i], element_length);
        current += element_length;
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