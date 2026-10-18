#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

static int append_segment(char ***result, size_t *capacity, size_t *count, const char *start, size_t len) {
    if (*count >= *capacity) {
        size_t new_cap = *capacity * 2;
        char **new_res = realloc(*result, new_cap * sizeof(char *));
        if (!new_res) {
            for (size_t j = 0; j < *count; j++) free((*result)[j]);
            free(*result);
            return -1;
        }
        *result = new_res;
        *capacity = new_cap;
    }
    (*result)[*count] = malloc(len + 1);
    if (!(*result)[*count]) {
        for (size_t j = 0; j < *count; j++) free((*result)[j]);
        free(*result);
        return -1;
    }
    memcpy((*result)[*count], start, len);
    (*result)[*count][len] = '\0';
    (*count)++;
    return 0;
}

char **split_uppercase(const char *input, size_t *count) {
    if (!input || !count) return NULL;
    *count = 0;
    size_t len = strlen(input);
    size_t capacity = 16;
    char **result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;

    size_t last_end = 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (c >= 'A' && c <= 'Z') {
            if (i > 0) {
                if (append_segment(&result, &capacity, count, input + last_end, i - last_end) != 0)
                    return NULL;
                last_end = i;
            }
        }
    }
    if (last_end < len) {
        if (append_segment(&result, &capacity, count, input + last_end, len - last_end) != 0)
            return NULL;
    }
    return result;
}

void free_split_result(char **result, size_t count) {
    if (result) {
        for (size_t i = 0; i < count; i++) free(result[i]);
        free(result);
    }
}

int main(void) {
    const char *test_string = "HelloWorldThisIsATest";
    size_t count = 0;
    char **parts = split_uppercase(test_string, &count);
    if (parts) {
        printf("Original: %s\n", test_string);
        printf("Split into %zu parts:\n", count);
        for (size_t i = 0; i < count; i++)
            printf("  [%zu]: %s\n", i, parts[i]);
        free_split_result(parts, count);
    } else {
        fprintf(stderr, "Failed to split string\n");
        return 1;
    }
    return 0;
}