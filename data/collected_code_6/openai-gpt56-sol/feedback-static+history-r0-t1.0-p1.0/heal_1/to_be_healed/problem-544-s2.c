#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    const char *first;
    const char *second;
} Tuple;

static int checked_add(size_t a, size_t b, size_t *result)
{
    if (result == NULL || a > SIZE_MAX - b) {
        return 0;
    }

    *result = a + b;
    return 1;
}

static char *flatten_tuple_list(const Tuple *tuples, size_t count)
{
    size_t length = 0;

    if (count > 0 && tuples == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        size_t first_length;
        size_t second_length;

        if (tuples[i].first == NULL || tuples[i].second == NULL) {
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

        if (!checked_add(length, first_length, &length) ||
            !checked_add(length, second_length, &length) ||
            !checked_add(length, 4, &length) ||
            (i + 1 < count && !checked_add(length, 2, &length))) {
            return NULL;
        }
    }

    if (!checked_add(length, 1, &length)) {
        return NULL;
    }

    char *result = malloc(length);
    if (result == NULL) {
        return NULL;
    }

    char *position = result;

    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t first_length = strlen(tuples[i].first);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t second_length = strlen(tuples[i].second);

        *position++ = '(';
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(position, tuples[i].first, first_length);
        position += first_length;
        *position++ = ',';
        *position++ = ' ';
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(position, tuples[i].second, second_length);
        position += second_length;
        *position++ = ')';

        if (i + 1 < count) {
            *position++ = ',';
            *position++ = ' ';
        }
    }

    *position = '\0';
    return result;
}

int main(void)
{
    const Tuple tuples[] = {
        {"apple", "red"},
        {"banana", "yellow"},
        {"grape", "purple"}
    };
    const size_t count = sizeof(tuples) / sizeof(tuples[0]);

    char *flattened = flatten_tuple_list(tuples, count);
    if (flattened == NULL) {
        fputs("Failed to flatten tuple list.\n", stderr);
        return EXIT_FAILURE;
    }

    if (puts(flattened) == EOF) {
        free(flattened);
        return EXIT_FAILURE;
    }

    free(flattened);
    return EXIT_SUCCESS;
}