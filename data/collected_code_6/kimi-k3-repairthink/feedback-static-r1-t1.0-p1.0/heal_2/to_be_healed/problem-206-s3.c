#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_STRING_LENGTH 4096

typedef struct {
    char **elements;
    size_t count;
} Tuple;

static size_t bounded_strlen(const char *s, size_t max_len, bool *terminated) {
    size_t len = 0;
    while (len < max_len) {
        if (s[len] == '\0') {
            *terminated = true;
            return len;
        }
        len++;
    }
    *terminated = false;
    return len;
}

char **adjacent_concat(const Tuple *t, size_t *out_count) {
    if (t == NULL || out_count == NULL || t->count == 0 || t->elements == NULL) {
        return NULL;
    }
    *out_count = 0;
    size_t n = t->count - 1;
    if (n > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }
    char **result = malloc(n * sizeof(char *));
    if (result == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        result[i] = NULL;
    }
    for (size_t i = 0; i < n; i++) {
        if (t->elements[i] == NULL || t->elements[i + 1] == NULL) {
            goto cleanup_fail;
        }
        bool term1 = false;
        bool term2 = false;
        size_t len1 = bounded_strlen(t->elements[i], MAX_STRING_LENGTH + 1, &term1);
        size_t len2 = bounded_strlen(t->elements[i + 1], MAX_STRING_LENGTH + 1, &term2);
        if (!term1 || !term2) {
            goto cleanup_fail;
        }
        if (len1 > SIZE_MAX - len2 - 1) {
            goto cleanup_fail;
        }
        size_t total = len1 + len2 + 1;
        result[i] = malloc(total);
        if (result[i] == NULL) {
            goto cleanup_fail;
        }
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i], t->elements[i], len1);
        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
        memcpy(result[i] + len1, t->elements[i + 1], len2);
        result[i][total - 1] = '\0';
    }
    *out_count = n;
    return result;

cleanup_fail:
    for (size_t j = 0; j < n; j++) {
        free(result[j]);
    }
    free(result);
    return NULL;
}

void free_result(char **result, size_t count) {
    if (result == NULL) {
        return;
    }
    for (size_t i = 0; i < count; i++) {
        free(result[i]);
    }
    free(result);
}

int main(void) {
    char *data[] = {"Hello", "World", "Foo", "Bar"};
    Tuple t = {data, 4};
    size_t out_count = 0;
    char **result = adjacent_concat(&t, &out_count);
    if (result == NULL) {
        fprintf(stderr, "Error: adjacent_concat failed\n");
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < out_count; i++) {
        printf("%s\n", result[i]);
    }
    free_result(result, out_count);
    return EXIT_SUCCESS;
}