#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

static size_t safe_strlen(const char *s, size_t maxlen) {
    size_t n = 0;
    while (n < maxlen && s[n] != '\0') {
        n++;
    }
    return n;
}

char **split_at_lowercase(const char *str, int *count) {
    if (!str || !count) {
        if (count) *count = 0;
        return NULL;
    }
    size_t len = safe_strlen(str, (size_t)INT_MAX + 1);
    if (len > (size_t)INT_MAX) {
        *count = 0;
        return NULL;
    }
    if (len == 0) {
        *count = 0;
        return NULL;
    }
    size_t max_parts = len + 1;
    char **parts = malloc(max_parts * sizeof(char *));
    if (!parts) {
        *count = 0;
        return NULL;
    }
    int part_count = 0;
    size_t start = 0;
    for (size_t i = 0; i <= len; i++) {
        if (i == len || islower((unsigned char)str[i])) {
            size_t part_len = i - start;
            if (part_len > 0) {
                char *part = malloc(part_len + 1);
                if (!part) {
                    for (int j = 0; j < part_count; j++) {
                        free(parts[j]);
                    }
                    free(parts);
                    *count = 0;
                    return NULL;
                }
                strncpy(part, str + start, part_len);
                part[part_len] = '\0';
                parts[part_count] = part;
                part_count++;
            }
            start = i + 1;
        }
    }
    *count = part_count;
    if (part_count == 0) {
        free(parts);
        return NULL;
    }
    return parts;
}

void free_split_result(char **parts, int count) {
    if (!parts) return;
    for (int i = 0; i < count; i++) {
        free(parts[i]);
    }
    free(parts);
}

int main(void) {
    const char *test_string = "HelloWorldThisIsATest";
    int count = 0;
    char **result = split_at_lowercase(test_string, &count);
    if (result) {
        printf("Split into %d parts:\n", count);
        for (int i = 0; i < count; i++) {
            printf("[%d]: '%s'\n", i, result[i]);
        }
        free_split_result(result, count);
    } else {
        printf("No parts or error occurred (count=%d)\n", count);
    }
    return 0;
}