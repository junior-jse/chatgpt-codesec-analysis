#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

char **split_at_lowercase(const char *str, size_t *count) {
    if (!str || !count) {
        return NULL;
    }

    /* Possible weaknesses found:
     * Flawfinder strlen: Does not handle strings that are not \0-terminated; if given one it may perform an over-read (it could cause a crash if unprotected) (CWE-126). (risk 1, buffer)
     */
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
        /* Possible weaknesses found:
         *  alloc_size is assigned 'token_len+1' here.
         */
        size_t alloc_size = token_len + 1;

        /* Possible weaknesses found:
         *  Condition 'alloc_size<=token_len' is always false [knownConditionTrueFalse]
         *  Condition 'alloc_size<=token_len' is always false
         */
        if (alloc_size <= token_len) {
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

        /* Possible weaknesses found:
         * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
         */
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

    /* Possible weaknesses found:
     *  final_size is assigned '*count+1' here.
     */
    size_t final_size = *count + 1;
    /* Possible weaknesses found:
     *  Condition 'final_size<=*count' is always false [knownConditionTrueFalse]
     *  Condition 'final_size<=*count' is always false
     */
    if (final_size <= *count) {
        for (size_t i = 0; i < *count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }

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