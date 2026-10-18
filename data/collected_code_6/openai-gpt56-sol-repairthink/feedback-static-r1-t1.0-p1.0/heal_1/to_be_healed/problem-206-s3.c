#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static bool concatenate_adjacent(const char *const input[], size_t count,
                                 char ***output, size_t *output_count)
{
    char **result = NULL;
    size_t result_count;

    if (input == NULL || output == NULL || output_count == NULL || count < 2) {
        return false;
    }

    *output = NULL;
    *output_count = 0;
    result_count = count - 1;

    if (result_count > SIZE_MAX / sizeof(*result)) {
        return false;
    }

    result = calloc(result_count, sizeof(*result));
    if (result == NULL) {
        return false;
    }

    for (size_t i = 0; i < result_count; ++i) {
        size_t first_length;
        size_t second_length;
        size_t total_length;

        if (input[i] == NULL || input[i + 1] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        first_length = strlen(input[i]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        second_length = strlen(input[i + 1]);

        if (first_length > SIZE_MAX - second_length - 1) {
            goto failure;
        }

        total_length = first_length + second_length + 1;
        result[i] = malloc(total_length);
        if (result[i] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], input[i], first_length);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i] + first_length, input[i + 1], second_length + 1);
    }

    *output = result;
    *output_count = result_count;
    return true;

failure:
    for (size_t i = 0; i < result_count; ++i) {
        free(result[i]);
    }
    free(result);
    return false;
}

static void free_strings(char **strings, size_t count)
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
    const char *tuple[] = {"a", "b", "c", "d"};
    const size_t tuple_count = sizeof(tuple) / sizeof(tuple[0]);
    char **result = NULL;
    size_t result_count = 0;

    if (!concatenate_adjacent(tuple, tuple_count, &result, &result_count)) {
        fputs("Failed to concatenate adjacent elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s%s", result[i], i + 1 < result_count ? " " : "\n") < 0) {
            free_strings(result, result_count);
            return EXIT_FAILURE;
        }
    }

    free_strings(result, result_count);
    return EXIT_SUCCESS;
}