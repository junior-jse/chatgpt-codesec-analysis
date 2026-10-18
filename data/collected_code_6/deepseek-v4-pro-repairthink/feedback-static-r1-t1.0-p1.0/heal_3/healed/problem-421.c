#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 4096

static int safe_strlen(const char *str) {
    int i;
    for (i = 0; i <= MAX_STRING_LEN; i++) {
        if (str[i] == '\0') {
            return i;
        }
    }
    return -1;
}

char **concat_elements(const char **tuple, int tuple_size, const char *delimiter, int *result_count) {
    if (tuple == NULL || delimiter == NULL || result_count == NULL) {
        return NULL;
    }

    if (tuple_size <= 1) {
        *result_count = 0;
        return NULL;
    }

    int pair_count = tuple_size - 1;
    char **result = malloc((size_t)pair_count * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;
    for (int i = 0; i < pair_count; i++) {
        if (tuple[i] == NULL || tuple[i + 1] == NULL) {
            goto cleanup;
        }

        int len1 = safe_strlen(tuple[i]);
        if (len1 < 0) {
            goto cleanup;
        }
        int len2 = safe_strlen(tuple[i + 1]);
        if (len2 < 0) {
            goto cleanup;
        }
        int delim_len = safe_strlen(delimiter);
        if (delim_len < 0) {
            goto cleanup;
        }

        size_t total_len = (size_t)len1 + (size_t)delim_len + (size_t)len2;

        char *combined = malloc(total_len + 1);
        if (combined == NULL) {
            goto cleanup;
        }

        int written = snprintf(combined, total_len + 1, "%s%s%s", tuple[i], delimiter, tuple[i + 1]);
        if (written < 0 || (size_t)written >= total_len + 1) {
            free(combined);
            goto cleanup;
        }

        result[count++] = combined;
        continue;

    cleanup:
        for (int j = 0; j < count; j++) {
            free(result[j]);
        }
        free(result);
        *result_count = 0;
        return NULL;
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