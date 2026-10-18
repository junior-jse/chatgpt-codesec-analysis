#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char **split_at_lowercase(const char *str, size_t *count) {
    if (!str || !count) {
        return NULL;
    }

    size_t len = strlen(str);
    size_t capacity = len > 0 ? len : 1;

    if (capacity > SIZE_MAX / sizeof(char *)) {
        return NULL;
    }

    char **result = malloc(capacity * sizeof(char *));
    if (!result) {
        return NULL;
    }

    *count = 0;
    const char *start = str;

    while (*start) {
        while (*start && islower((unsigned char)*start)) {
            start++;
        }
        if (!*start) {
            break;
        }

        const char *end = start;
        while (*end && !islower((unsigned char)*end)) {
            end++;
        }

        size_t token_len = end - start;
        size_t alloc_size = token_len + 1;

        if (token_len == SIZE_MAX) {
            for (size_t i = 0; i < *count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        char *token = malloc(alloc_size);
        if (!token) {
            for (size_t i = 0; i < *count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        memcpy(token, start, token_len);
        token[token_len] = '\0';

        if (*count >= capacity) {
            if (capacity > SIZE_MAX / 2 || capacity * 2 > SIZE_MAX / sizeof(char *)) {
                free(token);
                for (size_t i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                free(token);
                for (size_t i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
        }

        result[*count] = token;
        (*count)++;
        start = end;
    }

    if (*count == SIZE_MAX) {
        for (size_t i = 0; i < *count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

    size_t final_size = *count + 1;

    char **final_result = realloc(result, final_size * sizeof(char *));
    if (!final_result) {
        for (size_t i = 0; i < *count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    result = final_result;
    result[*count] = NULL;

    return result;
}

int main(void) {
    const char *input = "abcHELLOxyzWORLDpqr";
    size_t count = 0;
    
    char **tokens = split_at_lowercase(input, &count);
    if (!tokens) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (size_t i = 0; i < count; i++) {
        printf("%s\n", tokens[i]);
        free(tokens[i]);
    }
    free(tokens);

    return 0;
}