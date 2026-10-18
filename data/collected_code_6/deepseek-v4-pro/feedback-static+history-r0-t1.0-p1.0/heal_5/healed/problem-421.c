#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

char **concat_elements(const char **tuple, int tuple_size, const char *delimiter, int *result_count) {
    if (tuple == NULL || delimiter == NULL || result_count == NULL) {
        return NULL;
    }

    if (tuple_size <= 0) {
        *result_count = 0;
        return NULL;
    }

    for (int i = 0; i < tuple_size; i++) {
        if (tuple[i] == NULL) {
            *result_count = 0;
            return NULL;
        }
    }

    size_t delim_len = 0;
    const char *delim_ptr = delimiter;
    while (*delim_ptr != '\0') {
        delim_len++;
        delim_ptr++;
    }

    int max_elements = tuple_size - 1;
    char **result = (char **)malloc((size_t)max_elements * sizeof(char *));
    if (result == NULL) {
        *result_count = 0;
        return NULL;
    }

    int count = 0;

    for (int i = 0; i < max_elements; i++) {
        size_t len1 = 0;
        const char *ptr1 = tuple[i];
        while (*ptr1 != '\0') {
            len1++;
            ptr1++;
        }

        size_t len2 = 0;
        const char *ptr2 = tuple[i + 1];
        while (*ptr2 != '\0') {
            len2++;
            ptr2++;
        }

        if (len1 > SIZE_MAX - delim_len || len1 + delim_len > SIZE_MAX - len2) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        size_t total_len = len1 + delim_len + len2;

        char *combined = (char *)malloc(total_len + 1);
        if (combined == NULL) {
            for (int j = 0; j < count; j++) {
                free(result[j]);
            }
            free(result);
            *result_count = 0;
            return NULL;
        }

        char *dest = combined;
        size_t remaining = total_len;

        if (len1 <= remaining) {
            const char *src1 = tuple[i];
            for (size_t j = 0; j < len1; j++) {
                dest[j] = src1[j];
            }
            dest += len1;
            remaining -= len1;
        }

        if (delim_len <= remaining) {
            const char *src_delim = delimiter;
            for (size_t j = 0; j < delim_len; j++) {
                dest[j] = src_delim[j];
            }
            dest += delim_len;
            remaining -= delim_len;
        }

        if (len2 <= remaining) {
            const char *src2 = tuple[i + 1];
            for (size_t j = 0; j < len2; j++) {
                dest[j] = src2[j];
            }
            dest += len2;
            remaining -= len2;
        }

        *dest = '\0';
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
        fprintf(stderr, "Failed to concatenate elements\n");
        return 1;
    }

    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result[i]);
        free(result[i]);
    }
    free(result);

    return 0;
}