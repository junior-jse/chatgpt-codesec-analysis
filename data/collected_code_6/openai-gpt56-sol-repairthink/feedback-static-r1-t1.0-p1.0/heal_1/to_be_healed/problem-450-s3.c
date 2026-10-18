#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **extract_strings_by_length(const char *const strings[],
                                 size_t count,
                                 size_t length,
                                 size_t *result_count)
{
    char **result = NULL;
    size_t matches = 0;
    size_t index = 0;

    if (result_count == NULL || (count > 0 && strings == NULL)) {
        return NULL;
    }

    *result_count = 0;

    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (strings[i] != NULL && strlen(strings[i]) == length) {
            ++matches;
        }
    }

    if (matches == 0) {
        return NULL;
    }

    if (matches > SIZE_MAX / sizeof(*result)) {
        return NULL;
    }

    result = calloc(matches, sizeof(*result));
    if (result == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        if (strings[i] != NULL && strlen(strings[i]) == length) {
            size_t allocation_size;

            if (length == SIZE_MAX) {
                goto cleanup;
            }

            allocation_size = length + 1;
            result[index] = malloc(allocation_size);
            if (result[index] == NULL) {
                goto cleanup;
            }

            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(result[index], strings[i], allocation_size);
            ++index;
        }
    }

    *result_count = matches;
    return result;

cleanup:
    for (size_t i = 0; i < index; ++i) {
        free(result[i]);
    }
    free(result);
    return NULL;
}

void free_string_list(char **strings, size_t count)
{
    if (strings == NULL) {
        return;
    }

    for (size_t i = 0; i < count; ++i) {
        free(strings[i]);
    }

    free(strings);
}

int main(void)
{
    const char *values[] = {
        "apple",
        "pear",
        "grape",
        "plum",
        "melon",
        "kiwi"
    };
    const size_t target_length = 5;
    size_t result_count = 0;
    char **results = extract_strings_by_length(
        values,
        sizeof(values) / sizeof(values[0]),
        target_length,
        &result_count
    );

    if (results == NULL) {
        if (result_count == 0) {
            return EXIT_SUCCESS;
        }
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s\n", results[i]) < 0) {
            free_string_list(results, result_count);
            return EXIT_FAILURE;
        }
    }

    free_string_list(results, result_count);
    return EXIT_SUCCESS;
}