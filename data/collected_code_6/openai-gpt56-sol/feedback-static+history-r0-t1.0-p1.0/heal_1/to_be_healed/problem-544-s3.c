#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>

typedef struct {
    const char *first;
    const char *second;
} Tuple;

static int checked_add_size(size_t a, size_t b, size_t *result)
{
    if (result == NULL || a > SIZE_MAX - b) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static char *flatten_tuple_list(const Tuple *tuples, size_t count)
{
    const char *tuple_separator = ", ";
    const char *item_separator = ": ";
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    const size_t tuple_separator_length = strlen(tuple_separator);
    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    const size_t item_separator_length = strlen(item_separator);
    size_t total_length = 0;
    char *output;
    char *cursor;

    if (count > 0 && tuples == NULL) {
        errno = EINVAL;
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t first_length;
        size_t second_length;

        if (tuples[i].first == NULL || tuples[i].second == NULL) {
            errno = EINVAL;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        first_length = strlen(tuples[i].first);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        second_length = strlen(tuples[i].second);

        if (!checked_add_size(total_length, first_length, &total_length) ||
            !checked_add_size(total_length, item_separator_length, &total_length) ||
            !checked_add_size(total_length, second_length, &total_length) ||
            (i + 1 < count &&
             !checked_add_size(total_length, tuple_separator_length,
                               &total_length))) {
            errno = EOVERFLOW;
            return NULL;
        }
    }

    if (!checked_add_size(total_length, 1, &total_length)) {
        errno = EOVERFLOW;
        return NULL;
    }

    output = malloc(total_length);
    if (output == NULL) {
        return NULL;
    }

    cursor = output;

    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t length = strlen(tuples[i].first);

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(cursor, tuples[i].first, length);
        cursor += length;

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(cursor, item_separator, item_separator_length);
        cursor += item_separator_length;

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        length = strlen(tuples[i].second);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(cursor, tuples[i].second, length);
        cursor += length;

        if (i + 1 < count) {
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(cursor, tuple_separator, tuple_separator_length);
            cursor += tuple_separator_length;
        }
    }

    *cursor = '\0';
    return output;
}

int main(void)
{
    const Tuple tuples[] = {
        {"name", "Alice"},
        {"age", "30"},
        {"city", "London"}
    };
    const size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
    char *flattened = flatten_tuple_list(tuples, tuple_count);

    if (flattened == NULL) {
        perror("flatten_tuple_list");
        return EXIT_FAILURE;
    }

    if (puts(flattened) == EOF) {
        free(flattened);
        return EXIT_FAILURE;
    }

    free(flattened);
    return EXIT_SUCCESS;
}