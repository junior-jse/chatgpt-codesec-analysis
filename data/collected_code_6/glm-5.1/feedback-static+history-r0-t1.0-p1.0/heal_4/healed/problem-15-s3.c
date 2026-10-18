#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char **split_at_lowercase(const char *str, size_t *count) {
    size_t len = strnlen(str, SIZE_MAX);
    size_t capacity = len > 0 ? len : 1;
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
        if (token_len >= SIZE_MAX) {
            for (size_t i = 0; i < *count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        char *token = malloc(token_len + 1);
        if (!token) {
            for (size_t i = 0; i < *count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }

        if (token_len > 0) {
            memcpy(token, start, token_len);
        }
        token[token_len] = '\0';

        if (*count >= capacity) {
            if (capacity == 0) capacity = 1;
            if (capacity > SIZE_MAX / 2) {
                free(token);
                for (size_t i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            size_t new_capacity = capacity * 2;
            char **new_result = realloc(result, new_capacity * sizeof(char *));
            if (!new_result) {
                free(token);
                for (size_t i = 0; i < *count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = new_result;
            capacity = new_capacity;
        }

        result[*count] = token;
        (*count)++;
        start = end;
    }

    char **final_result = realloc(result, (*count + 1) * sizeof(char *));
    if (final_result) {
        result = final_result;
    }
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