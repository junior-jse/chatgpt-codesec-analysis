#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int concatenate_adjacent(const char *const items[],
                                size_t count,
                                char ***result,
                                size_t *result_count)
{
    char **output = NULL;
    size_t output_count;

    if (items == NULL || result == NULL || result_count == NULL) {
        return -1;
    }

    *result = NULL;
    *result_count = 0;

    if (count < 2) {
        return 0;
    }

    output_count = count - 1;

    if (output_count > SIZE_MAX / sizeof(*output)) {
        return -1;
    }

    output = calloc(output_count, sizeof(*output));
    if (output == NULL) {
        return -1;
    }

    for (size_t i = 0; i < output_count; ++i) {
        size_t first_length;
        size_t second_length;
        size_t total_length;

        if (items[i] == NULL || items[i + 1] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        first_length = strlen(items[i]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        second_length = strlen(items[i + 1]);

        if (first_length > SIZE_MAX - second_length - 1) {
            goto failure;
        }

        total_length = first_length + second_length + 1;
        output[i] = malloc(total_length);
        if (output[i] == NULL) {
            goto failure;
        }

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(output[i], items[i], first_length);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(output[i] + first_length, items[i + 1], second_length + 1);
    }

    *result = output;
    *result_count = output_count;
    return 0;

failure:
    for (size_t i = 0; i < output_count; ++i) {
        free(output[i]);
    }
    free(output);
    return -1;
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
    const char *tuple[] = {"ab", "cd", "ef", "gh"};
    const size_t tuple_count = sizeof(tuple) / sizeof(tuple[0]);
    char **result = NULL;
    size_t result_count = 0;

    if (concatenate_adjacent(tuple, tuple_count, &result, &result_count) != 0) {
        fputs("Failed to concatenate adjacent elements.\n", stderr);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < result_count; ++i) {
        if (printf("%s\n", result[i]) < 0) {
            free_strings(result, result_count);
            return EXIT_FAILURE;
        }
    }

    free_strings(result, result_count);
    return EXIT_SUCCESS;
}