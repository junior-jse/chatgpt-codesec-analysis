#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings(const char *const *values, size_t count,
                       size_t start, size_t length)
{
    char **result;
    size_t i;

    if (values == NULL || start > count || length > count - start) {
        return NULL;
    }

    if (length > (SIZE_MAX / sizeof(*result)) - 1) {
        return NULL;
    }

    result = calloc(length + 1, sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (i = 0; i < length; ++i) {
        size_t string_length;

        if (values[start + i] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        string_length = strlen(values[start + i]);
        if (string_length == SIZE_MAX) {
            goto failure;
        }

        result[i] = malloc(string_length + 1);
        if (result[i] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], values[start + i], string_length + 1);
    }

    return result;

failure:
    for (i = 0; i < length; ++i) {
        free(result[i]);
    }
    free(result);
    return NULL;
}

void free_strings(char **strings, size_t count)
{
    size_t i;

    if (strings == NULL) {
        return;
    }

    for (i = 0; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
}

int main(void)
{
    const char *values[] = {
        "alpha",
        "beta",
        "gamma",
        "delta",
        "epsilon"
    };
    const size_t value_count = sizeof(values) / sizeof(values[0]);
    const size_t start = 1;
    const size_t length = 3;
    char **extracted;
    size_t i;

    extracted = extract_strings(values, value_count, start, length);
    if (extracted == NULL) {
        fputs("Failed to extract strings.\n", stderr);
        return EXIT_FAILURE;
    }

    for (i = 0; i < length; ++i) {
        if (puts(extracted[i]) == EOF) {
            free_strings(extracted, length);
            return EXIT_FAILURE;
        }
    }

    free_strings(extracted, length);
    return EXIT_SUCCESS;
}