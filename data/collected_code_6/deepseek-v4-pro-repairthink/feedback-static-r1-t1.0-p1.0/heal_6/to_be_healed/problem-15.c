#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_lowercase(const char *str, int *count) {
    if (!str || !count) return NULL;

    size_t capacity = 16;
    char **parts = malloc(capacity * sizeof(char *));
    if (!parts) return NULL;

    size_t part_count = 0;
    const char *start = str;
    const char *p = str;

    while (*p) {
        if (islower((unsigned char)*p)) {
            size_t part_len = p - start;
            char *segment = malloc(part_len + 1);
            if (!segment) {
                for (size_t i = 0; i < part_count; i++) free(parts[i]);
                free(parts);
                return NULL;
            }
            /* Possible weaknesses found:
             * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
             */
            memcpy(segment, start, part_len);
            segment[part_len] = '\0';

            if (part_count == capacity) {
                capacity *= 2;
                char **tmp = realloc(parts, capacity * sizeof(char *));
                if (!tmp) {
                    free(segment);
                    for (size_t i = 0; i < part_count; i++) free(parts[i]);
                    free(parts);
                    return NULL;
                }
                parts = tmp;
            }
            parts[part_count++] = segment;
            start = p + 1;
        }
        p++;
    }

    size_t remaining_len = p - start;
    char *segment = malloc(remaining_len + 1);
    if (!segment) {
        for (size_t i = 0; i < part_count; i++) free(parts[i]);
        free(parts);
        return NULL;
    }
    /* Possible weaknesses found:
     * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
     */
    memcpy(segment, start, remaining_len);
    segment[remaining_len] = '\0';

    if (part_count == capacity) {
        capacity *= 2;
        char **tmp = realloc(parts, capacity * sizeof(char *));
        if (!tmp) {
            free(segment);
            for (size_t i = 0; i < part_count; i++) free(parts[i]);
            free(parts);
            return NULL;
        }
        parts = tmp;
    }
    parts[part_count++] = segment;

    *count = part_count;
    return parts;
}

int main(void) {
    const char *test_string = "HelloWorld thisIsATest";
    int count = 0;
    char **result = split_at_lowercase(test_string, &count);
    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for (int i = 0; i < count; i++) {
        printf("[%s]\n", result[i]);
        free(result[i]);
    }
    free(result);
    return 0;
}