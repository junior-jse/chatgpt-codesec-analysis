#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **concat_elements(const char **tuple, int tuple_size, const char *delimiter, int *result_count) {
    if (tuple == NULL || delimiter == NULL || result_count == NULL) {
        return NULL;
    }

    if (tuple_size <= 1) {
        *result_count = 0;
        return NULL;
    }

    int pair_count = tuple_size - 1;
    char **result = malloc(pair_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < pair_count; i++) {
        if (tuple[i] == NULL || tuple[i + 1] == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len1 = strlen(tuple[i]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t len2 = strlen(tuple[i + 1]);
        /* Possible weaknesses found:
         * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
         */
        size_t delim_len = strlen(delimiter);
        size_t total_len = len1 + delim_len + len2;

        char *combined = malloc(total_len + 1);
        if (combined == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        int written = snprintf(combined, total_len + 1, "%s%s%s", tuple[i], delimiter, tuple[i + 1]);
        if (written < 0 || (size_t)written >= total_len + 1) {
            free(combined);
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        result[count++] = combined;
    }

    *result_count = count;
    return result;
}

int main(void) {
    const char *tuple[] = {"hello", "world", "foo", "bar"};
    int result_count = 0;
    char **result = concat_elements(tuple, 4, "-", &result_count);

    if (result == NULL) {
        if (result_count > 0) {
            fprintf(stderr, "Failed to concatenate elements\n");
            return 1;
        }
        return 0;
    }

    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}