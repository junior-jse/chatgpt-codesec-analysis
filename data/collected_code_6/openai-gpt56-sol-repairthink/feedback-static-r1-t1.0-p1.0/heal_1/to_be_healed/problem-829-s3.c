#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static char *duplicate_string(const char *source)
{
    size_t length;
    char *copy;

    if (source == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
    length = strlen(source);
    if (length == SIZE_MAX) {
        return NULL;
    }

    copy = malloc(length + 1U);
    if (copy == NULL) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(copy, source, length + 1U);
    return copy;
}

static int find_second_most_frequent(const char *const strings[],
                                     size_t count,
                                     const char **result)
{
    char **unique_strings = NULL;
    size_t *frequencies = NULL;
    size_t unique_count = 0U;
    size_t highest = 0U;
    size_t second_highest = 0U;
    size_t second_index = 0U;
    size_t i;
    int status = -1;

    if (strings == NULL || result == NULL || count == 0U) {
        return -1;
    }

    *result = NULL;

    if (count > SIZE_MAX / sizeof(*unique_strings) ||
        count > SIZE_MAX / sizeof(*frequencies)) {
        return -1;
    }

    unique_strings = calloc(count, sizeof(*unique_strings));
    frequencies = calloc(count, sizeof(*frequencies));

    if (unique_strings == NULL || frequencies == NULL) {
        goto cleanup;
    }

    for (i = 0U; i < count; ++i) {
        size_t j;

        if (strings[i] == NULL) {
            goto cleanup;
        }

        for (j = 0U; j < unique_count; ++j) {
            if (strcmp(strings[i], unique_strings[j]) == 0) {
                ++frequencies[j];
                break;
            }
        }

        if (j == unique_count) {
            unique_strings[unique_count] = duplicate_string(strings[i]);
            if (unique_strings[unique_count] == NULL) {
                goto cleanup;
            }

            frequencies[unique_count] = 1U;
            ++unique_count;
        }
    }

    for (i = 0U; i < unique_count; ++i) {
        if (frequencies[i] > highest) {
            highest = frequencies[i];
        }
    }

    for (i = 0U; i < unique_count; ++i) {
        if (frequencies[i] < highest &&
            frequencies[i] > second_highest) {
            second_highest = frequencies[i];
            second_index = i;
        }
    }

    if (second_highest == 0U) {
        status = 0;
        goto cleanup;
    }

    *result = duplicate_string(unique_strings[second_index]);
    if (*result == NULL) {
        goto cleanup;
    }

    status = 1;

cleanup:
    if (unique_strings != NULL) {
        for (i = 0U; i < unique_count; ++i) {
            free(unique_strings[i]);
        }
    }

    free(unique_strings);
    free(frequencies);
    return status;
}

int main(void)
{
    const char *strings[] = {
        "apple", "banana", "apple", "orange", "banana",
        "apple", "grape", "banana", "orange", "banana"
    };
    const size_t count = sizeof(strings) / sizeof(strings[0]);
    const char *result = NULL;
    int status;

    status = find_second_most_frequent(strings, count, &result);

    if (status < 0) {
        fputs("Unable to determine the result.\n", stderr);
        return EXIT_FAILURE;
    }

    if (status == 0) {
        puts("No second most frequent string exists.");
        return EXIT_SUCCESS;
    }

    printf("%s\n", result);
    free((void *)result);
    return EXIT_SUCCESS;
}