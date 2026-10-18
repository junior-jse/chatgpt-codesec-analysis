#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char **split_at_lowercase(const char *str, size_t *count) {
    if (str == NULL || count == NULL) {
        return NULL;
    }

    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }

    if (len == 0) {
        *count = 0;
        return NULL;
    }

    size_t max_parts = 1;
    for (size_t i = 0; i < len; i++) {
        if (islower((unsigned char)str[i])) {
            max_parts++;
        }
    }

    if (max_parts > SIZE_MAX / sizeof(char *)) {
        *count = 0;
        return NULL;
    }

    char **result = calloc(max_parts, sizeof(char *));
    if (result == NULL) {
        *count = 0;
        return NULL;
    }

    size_t part_count = 0;
    size_t start = 0;

    for (size_t i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            size_t part_len = i - start;

            if (part_len == SIZE_MAX) {
                for (size_t j = 0; j < part_count; j++) {
                    free(result[j]);
                }
                free(result);
                *count = 0;
                return NULL;
            }

            result[part_count] = malloc(part_len + 1);
            if (result[part_count] == NULL) {
                for (size_t j = 0; j < part_count; j++) {
                    free(result[j]);
                }
                free(result);
                *count = 0;
                return NULL;
            }

            if (part_len > 0) {
                for (size_t k = 0; k < part_len; k++) {
                    result[part_count][k] = str[start + k];
                }
            }
            result[part_count][part_len] = '\0';
            part_count++;
            start = i;
        }
    }

    *count = part_count;
    return result;
}

void free_split_result(char **result, size_t count) {
    if (result == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    const char *test_string = "HelloWorldThisIsATest";
    size_t count = 0;

    char **parts = split_at_lowercase(test_string, &count);

    if (parts == NULL && count > 0) {
        fprintf(stderr, "Memory allocation failed\n");
        return EXIT_FAILURE;
    }

    printf("Original string: %s\n", test_string);
    printf("Split into %zu parts:\n", count);

    for (size_t i = 0; i < count; i++) {
        printf("Part %zu: \"%s\"\n", i + 1, parts[i]);
    }

    free_split_result(parts, count);

    return EXIT_SUCCESS;
}